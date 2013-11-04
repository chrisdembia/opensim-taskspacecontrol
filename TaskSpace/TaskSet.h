#ifndef OPENSIM_TASKSPACE_TASKSET_H_
#define OPENSIM_TASKSPACE_TASKSET_H_

namespace OpenSim {

namespace TaskSpace {

class OSIMTASKSPACE_API TaskSet : public OpenSim::Set<TaskSet>
{
OpenSim_DECLARE_CONCRETE_CLASS(TaskSet, OpenSim::Set<TaskSet>);

};

} // namespace TaskSpace

} // namespace OpenSim

#endif