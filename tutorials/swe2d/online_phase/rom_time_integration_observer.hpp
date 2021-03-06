
#ifndef PRESSIO_TUT_ROM_TIME_INTEGRATION_OBSERVER_HPP_
#define PRESSIO_TUT_ROM_TIME_INTEGRATION_OBSERVER_HPP_

template <typename rom_state_t, typename native_state_t>
struct observer
{
  std::ofstream myfile_;
  std::ofstream myRefFile_;

  observer(const native_state_t & y0)
    : myfile_("solution.bin",  std::ios::out | std::ios::binary),
      myRefFile_("state_ref.bin",  std::ios::out | std::ios::binary)
  {
    for (int i =0; i < y0.size(); i++){
      myRefFile_.write(reinterpret_cast<const char*>(&y0(i)),sizeof(y0(i)));
    }
    myRefFile_.close();
  }

  void operator()(size_t step,
  		  double t,
  		  const rom_state_t & y){
    const auto & ydata = *y.data();
    for (int i=0;i<y.extent(0);i++){
      myfile_.write(reinterpret_cast<const char*>(&ydata(i)),sizeof(ydata(i)));
    }
    std::cout << "Time = " << t << std::endl;
  }

  void closeFile(){
    myfile_.close();
  }
};

#endif
