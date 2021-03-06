#include "PriorityLevel.h"

#include "TaskSet.h"

#include <OpenSim/Simulation/Model/Model.h>

using SimTK::FactorLU;
using SimTK::Matrix;
using SimTK::State;
using SimTK::Vector;

using namespace OpenSim;

TaskSpace::PriorityLevel::PriorityLevel()
{
    setNull();
    constructProperties();
}

void TaskSpace::PriorityLevel::setNull()
{
    m_model = NULL;
}

void TaskSpace::PriorityLevel::constructProperties()
{
    constructProperty_tasks(TaskSet());
}

Vector TaskSpace::PriorityLevel::generalizedForces(const State& s) const
{
    return jacobian(s).transpose() * taskSpaceForces(s);
}

Matrix TaskSpace::PriorityLevel::nullspaceProjection(const State& s) const
{
    // Build identity matrix.
    Matrix identity(s.getNU(), s.getNU());
    identity.setToZero();
    identity.diag().setTo(1.0);

    return identity - dynamicallyConsistentJacobianInverse(s) * jacobian(s);
}

Vector TaskSpace::PriorityLevel::taskSpaceForces(const State& s) const
{
    Vector levelTaskForces(getNumScalarTasks());

    // Used to write constituent task force vectors to the correct place.
    unsigned int STidx = 0;

    for (unsigned int iT = 0; iT < get_tasks().getSize(); iT++)
    {
        unsigned int nST = get_tasks().get(iT).getNumScalarTasks();

        // Write the nST x 1 matrix (vector) to the (STidx, 0) location.
        levelTaskForces.updBlock(STidx, 0, nST, 1) =
            get_tasks().get(iT).taskSpaceForces(s);

        STidx += nST;
    }
    return levelTaskForces;
}

Matrix TaskSpace::PriorityLevel::jacobian(const State& s) const
{
    Matrix levelJacobian(getNumScalarTasks(), s.getNU());

    // Used to write constituent jacobians to the correct place.
    unsigned int STidx = 0;

    for (unsigned int iT = 0; iT < get_tasks().getSize(); iT++)
    {
        unsigned int nST = get_tasks().get(iT).getNumScalarTasks();

        // Write the nST x NU matrix to the (STidx, 0) location.
        levelJacobian.updBlock(STidx, 0, nST, s.getNU()) =
            get_tasks().get(iT).jacobian(s);

        STidx += nST;
    }
    return levelJacobian;
}

Matrix TaskSpace::PriorityLevel::dynamicallyConsistentJacobianInverse(
        const State& s) const
{
    // J^T \Lambda
    // -----------
    Matrix jacobianTransposeTimesLambda =
        jacobian(s).transpose() * taskSpaceMassMatrix(s);

    // A^{-1} J^T \Lambda
    // ------------------
    Matrix dynConsistentJacobianInverse(s.getNU(), getNumScalarTasks());

    for (unsigned int iST = 0; iST < getNumScalarTasks(); ++iST)
    {
        m_model->getMatterSubsystem().multiplyByMInv(s,
                jacobianTransposeTimesLambda.col(iST),
                dynConsistentJacobianInverse.updCol(iST));
    }

    return dynConsistentJacobianInverse;
}

Matrix TaskSpace::PriorityLevel::taskSpaceMassMatrix(const State& s) const
{
    // A^{-1} J^T
    // -------------
    Matrix jac = jacobian(s);
    Matrix jacobianTranspose = jac.transpose();
    Matrix systemMassMatrixInverseTimesJacobianTranspose(
            s.getNU(), getNumScalarTasks());

    for (unsigned int iST = 0; iST < getNumScalarTasks(); ++iST)
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
    for (unsigned int iT = 0; iT < get_tasks().getSize(); ++iT)
    {
        get_tasks().get(iT).setModel(model);
        m_numScalarTasks += get_tasks().get(iT).getNumScalarTasks();
    }
}




