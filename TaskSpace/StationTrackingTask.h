#ifndef OPENSIM_TASKSPACE_STATIONTRACKINGTASK_H_
#define OPENSIM_TASKSPACE_STATIONTRACKINGTASK_H_

#include "StationTask.h"

#include "osimTaskSpaceControlDLL.h"

namespace OpenSim {

namespace TaskSpace {

/**
 * TODO
 */
class OSIMTASKSPACECONTROL_API StationTrackingTask : public TaskSpace::StationTask
{
OpenSim_DECLARE_ABSTRACT_OBJECT(TaskSpace::StationTrackingTask,
        TaskSpace::Task);
public:

    /** @name Property declarations */
    /**@{**/
    OpenSim_DECLARE_PROPERTY(proportional_gain, double,
            "Proportional gain (kp).");
    OpenSim_DECLARE_PROPERTY(derivative_gain, double,
            "Derivative gain (kd).");
    /**@}**/


    StationTrackingTask();


    // -------------------------------------------------------------------------
    // Specification of StationTrackingTask interface
    // -------------------------------------------------------------------------

    /**
     * @brief Expressed in ground.
     */
    virtual Vec3 desiredLocation(const State& s) const = 0;

    /**
     * @brief Expressed in ground. Must be consistent with desired location.
     */
    virtual Vec3 desiredVelocity(const State& s) const = 0;

    /**
     * @brief Expressed in ground. Must be consistent with desired velocity.
     */
    virtual Vec3 desiredAcceleration(const State& s) const = 0;

    // -------------------------------------------------------------------------
    // Implementation of StationTask interface
    // -------------------------------------------------------------------------

    virtual Vec3 controlLaw(const State& s) const OVERRIDE_11 final;

private:

    void constructProperties();

};

} // namespace TaskSpace

} // namespace OpenSim

#endif
