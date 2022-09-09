
#ifndef PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_OBSERVER_HPP_
#define PRESSIO_TUTORIALS_ENDTOEND_WORKFLOW_OBSERVER_HPP_

class StateObserver
{
public:
  StateObserver(const std::string & f0, int freq)
    : myfile_(f0,  std::ios::out | std::ios::binary),
      sampleFreq_(freq){}

  explicit StateObserver(int freq)
    : myfile_("state_snapshots.bin",  std::ios::out | std::ios::binary),
      sampleFreq_(freq){}

  ~StateObserver(){ myfile_.close(); }

  template<typename TimeType, typename ObservableType>
  std::enable_if_t< pressio::is_vector_eigen<ObservableType>::value >
  operator()(pressio::ode::StepCount step,
	     const TimeType /*timeIn*/,
	     const ObservableType & state)
  {
    if (step.get() % sampleFreq_ == 0){
      const std::size_t ext = state.size()*sizeof(typename ObservableType::Scalar);
      myfile_.write(reinterpret_cast<const char*>(&state(0)), ext);
    }
  }

private:
  std::ofstream myfile_;
  const int sampleFreq_ = {};
};

class RhsObserver
{
public:
  RhsObserver(const std::string & f0, int freq)
    : myfile_(f0,  std::ios::out | std::ios::binary),
      sampleFreq_(freq){}

  explicit RhsObserver(int freq)
    : myfile_("rhs_snapshots.bin", std::ios::out | std::ios::binary),
      sampleFreq_(freq){}

  ~RhsObserver(){ myfile_.close(); }

  template<typename TimeType, typename ObservableType>
  std::enable_if_t< pressio::is_vector_eigen<ObservableType>::value >
  operator()(pressio::ode::StepCount step,
	     pressio::ode::IntermediateStepCount /*imStepCount*/,
	     const TimeType /*timeIn*/,
	     const ObservableType & rhs)
  {
    if (step.get() % sampleFreq_ == 0){
      const std::size_t ext = rhs.size()*sizeof(typename ObservableType::Scalar);
      myfile_.write(reinterpret_cast<const char*>(&rhs(0)), ext);
    }
  }

private:
  std::ofstream myfile_;
  const int sampleFreq_ = {};
};

#endif
