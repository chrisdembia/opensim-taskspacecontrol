/* A robotic arm starts from rest and we instruct its end-effector to move to a
 * 'goal position' (in Khatib's lingo).
 *
 * */

#include <OpenSim/OpenSim.h>

#include <TaskSpace/Controller.h>
#include <TaskSpace/TaskSet.h>
// TODO#include <TaskSpace/ConstantStationTrackingTask.h>
#include <TaskSpace/GravityCompensationTask.h>
#include <TaskSpace/PriorityLevel.h>
#include <TaskSpace/PriorityLevelSet.h>

#include <iostream>

using namespace OpenSim;
using namespace SimTK;

int main()
{
    // Instantiate objects.
    // --------------------
    Model model("fourlinks.osim");
    model.setUseVisualizer(true);

    TaskSpace::Controller * controller = new TaskSpace::Controller();
    controller->setName("goal_position");
    controller->setActuators(model.updActuators());

    // Define the tasks.
    // -----------------
    TaskSpace::GravityCompensationTask * gravComp =
        new TaskSpace::GravityCompensationTask();
    TaskSpace::PriorityLevel * priorityLevel1 =
        new TaskSpace::PriorityLevel();
    controller->upd_priority_levels().adoptAndAppend(priorityLevel1);
    priorityLevel1->upd_tasks().adoptAndAppend(gravComp);

    /*
    TaskSpace::ConstantStationTrackingTask * endEffector =
        new TaskSpace::ConstantStationTrackingTask();
    endEffector->set_body_name("link4");
    endEffector->set_location_in_body(Vec3(0.1, 0.1, 0.0));
    endEffector->set_desired_location(Vec3(0.6, 1.0, 0.0));
    TaskSpace::PriorityLevel * priorityLevel1 =
        new TaskSpace::PriorityLevel();
    controller->upd_priority_levels().adoptAndAppend(priorityLevel1);
    priorityLevel1->upd_tasks().adoptAndAppend(endEffector);

    TaskSpace::ConstantStationTrackingTask * link2 =
        new TaskSpace::ConstantStationTrackingTask();
    link2->set_body_name("link2");
    link2->set_location_in_body(Vec3(0.5, 0.1, 0.0));
    link2->set_desired_location(Vec3(0.2, 1.5, 0.0));
    priorityLevel1->upd_tasks().adoptAndAppend(link2);
    // TODO TaskSpace::PriorityLevel * priorityLevel2 =
    // TODO     new TaskSpace::PriorityLevel();
// TODO    controller->upd_priority_levels().adoptAndAppend(priorityLevel2);
*/

    model.addController(controller);

    // Prepare to simulate.
    // --------------------
    State& initState = model.initSystem();
    RungeKuttaMersonIntegrator integrator(model.getMultibodySystem());
    Manager manager(model, integrator);
    manager.setInitialTime(0.0);
    manager.setFinalTime(5.0);
    model.printDetailedInfo(initState, std::cout);

    // Simulate.
    // ---------
    // TODO std::cout << "Press Enter to start simulation..." << std::endl;
    // TODO getchar();
    manager.integrate(initState);

    // Save data.
    // ----------
    model.printControlStorage("fourlinks_controls.sto");
    manager.getStateStorage().print("fourlinks_states.sto");

    model.print("fourlinks_with_controller.osim");
}

/***
using SimTK::RungeKuttaMersonIntegrator;
using SimTK::State;
using SimTK::Vec3;

using OpenSim::Manager;
using OpenSim::Model;

using OpenSim::TaskSpace;
using TaskSpace::PositionTask;
using TaskSpace::PriorityLevel;

class RRRGoalPositionTask : public PositionTask
{
OpenSim_DECLARE_CONCRETE_OBJECT(RRRGoalPositionTask, PositionTask);
public:

    Vec3 desiredPosition(s) const
    {
        return Vec3(2.5, 2.0, 1.0);
    }

    Vec3 desiredVelocity(s) const
    {
        return Vec3(0.0, 0.0, 0.0);
    }

    Vec3 desiredAcceleration(s) const
    {
        return Vec3(0.0, 0.0, 0.0);
    }

};

// TODO set time step, or interval on which data is output.
int main(int argc, char * argv[])
{
    // Instantiate objects.
    // --------------------
    Model model("rrr_robot.osim");
    TaskSpace::Controller * controller = new TaskSpace::Controller();
    controller->setName("RRR_goal_position");
    controller->setActuators(model.updActuators());
    model.addController(controller);

    // Define the goal position task.
    // ------------------------------
    RRRGoalPositionTask * goalPosition;
    goalPosition->set_body_name("link3");
    goalPosition->set_position_on_body(Vec3(1.0, 0.0, 0.0));
    PriorityLevel& priorityLevel = controller->getPriorityLevelSet().get(0);
    priorityLevel.updTaskSet().adoptAndAppend(goalPosition);

    // Prepare to simulate.
    // --------------------
    State& initState = model.initSystem();
    RungeKuttaMersonIntegrator integrator(model.getMultibodySystem());
    Manager manager(model, integrator);
    manager.setInitialTime(0.0);
    manager.setFinalTime(5.0);
    model.printDetailedInfo(initState, std::cout);

    // Simulate.
    // ---------
    manager.integrate(initState);

    // Save data.
    // ----------
    model.printControlStorage("testRRR_controls.sto");
    manager.getStateStorage().print("testRRR_states.sto");

    // Compare output to correct desired output.
    // -----------------------------------------
    // TODO

    return EXIT_SUCCESS;

}
*/
