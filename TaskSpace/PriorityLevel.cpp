#include "PriorityLevel.h"

#include <OpenSim/Simulation/Model/Model.h>

using SimTK::FactorLU;
using SimTK::Matrix;
using SimTK::State;
using SimTK::Vector;

using namespace OpenSim;

TaskSpace::PriorityLevel::PriorityLevel()
{
    setNull();
    constructInfrastructure();
}

void TaskSpace::PriorityLevel::setNull()
{
    m_model = NULL;
}

void TaskSpace::PriorityLevel::constructProperties()
{
    constructProperty_tasks();
}

void TaskSpace::PriorityLevel::connect(Component& root) OVERRIDE_11
{
    Super::connect(root);
    // TODO
}

void TaskSpace::PriorityLevel::constructInputs() OVERRIDE_11
{
}

void TaskSpace::PriorityLevel::constructOutputs() OVERRIDE_11
{
    // TODO generalizedForces->taskSpaceForces.
    constructOutput<Matrix>("nullspaceProjection",
            std::bind(&TaskSpace::PriorityLevel::nullspaceProjection,
                this, std::placeholders::_1),
            SimTK::Stage::Position);
}

// TODO this makes no sense. the priority level must collect
// taskSpaceForce's...can't concatenate generalized forces.
Vector TaskSpace::PriorityLevel::generalizedForces(const State& s)
{
    /* TODO
    Vector levelGenForces(getNumScalarTasks());

    // Used to write constituent generalized force vectors to the correct
    // place.
    unsigned int STidx = 0;

    for (unsigned int iT = 0; iT < get_tasks().getSize(); iT++)
    {
        unsigned int nST = get_tasks().get(iT).getNumScalarTasks();

        // Write the nST x 1 matrix (vector) to the (STidx, 0) location.
        levelGenForces.updBlock(STidx, 0, nST, 1) =
            get_tasks().get(iT).generalizedForces(s);

        STidx += nST;
    }
    return levelGenForces;
    */
    return get_tasks().get(0).generalizedForces(s);
}

Matrix TaskSpace::PriorityLevel::nullspaceProjection(const State& s)
{
    // Build identity matrix.
    Matrix identity(s.getNU(), s.getNU());
    identity.setToZero();
    identity.diag().setTo(1.0);

    return identity - dynamicallyConsistentJacobianInverse(s) * jacobian(s);
}

Matrix TaskSpace::PriorityLevel::jacobian(const State& s)
{
    Matrix levelJacobian(getNumScalarTasks(), s.getNU());

    // Used to write constituent jacobians to the correct place.
    unsigned int STidx = 0;

    for (unsigned int iT = 0; iT < get_tasks().size(); iT++)
    {
        unsigned int nST = get_tasks()[iT].getNumScalarTasks();

        // Write the nST x NU matrix to the (STidx, 0) location.
        levelJacobian.updBlock(STidx, 0, nST, s.getNU()) =
            get_tasks()[iT].jacobian(s);

        STidx += nST;
    }
    return levelJacobian;
}

Matrix TaskSpace::PriorityLevel::dynamicallyConsistentJacobianInverse(
        const State& s)
{
    // J^T \Lambda
    // -----------
    Matrix jacobianTransposeTimesLambda =
        jacobian(s).transpose() * taskSpaceMassMatrix(s);

    // A^{-1} J^T \Lambda
    // ------------------
    Matrix dynConsistentJacobianInverse(s.getNU(), getNumScalarTasks());

    for (unsigned int iST = 0; iST < getNumScalarTasks(); iST++)
    {
        m_model->getMatterSubsystem().multiplyByMInv(s,
                jacobianTransposeTimesLambda.col(iST),
                dynConsistentJacobianInverse.updCol(iST));
    }

    return dynConsistentJacobianInverse;
}

Matrix TaskSpace::PriorityLevel::taskSpaceMassMatrix(const State& s)
{
    // A^{-1} J^T
    // -------------
    Matrix jac = jacobian(s);
    Matrix jacobianTranspose = jac.transpose();
    Matrix systemMassMatrixInverseTimesJacobianTranspose(
            s.getNU(), getNumScalarTasks());

    for (unsigned int iST = 0; iST < getNumScalarTasks(); iST++)
    {
        m_model->getMatterSubsystem().multiplyByMInv(s,
                jacobianTranspose.col(iST),
                systemMassMatrixInverseTimesJacobianTranspose.updCol(iST));
    }

    // J A^{-1} J^T
    // -------------
    Matrix taskMassMatrixInverse =
        jac * systemMassMatrixInverseTimesJacobianTranspose;

    // (J A^{-1} J^T)^{-1}
    // -------------------
    // TODO compute inverse in a different way; this feels messy.
    FactorLU taskMassMatrixInverseLU(taskMassMatrixInverse);
    Matrix taskMassMatrix(getNumScalarTasks(), getNumScalarTasks());
    taskMassMatrixInverseLU.inverse(taskMassMatrix);

    return taskMassMatrix;
}

void TaskSpace::PriorityLevel::setModel(const Model& model)
{
    m_model = &model;

    m_numScalarTasks = 0;
    for (unsigned int iT = 0; iT < get_tasks().size(); iT++)
    {
        get_tasks()[iT].setModel(model);
        m_numScalarTasks += get_tasks()[iT].getNumScalarTasks();
    }
}




