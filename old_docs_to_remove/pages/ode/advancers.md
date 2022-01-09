
# How to use ode advancers

Suppose that you have:

```cpp
@codesnippet
../../../tutorials/ode_advancer.cc
52:95
```


## advance_n_times

```cpp
@codesnippet
../../../tutorials/ode_advancer.cc
98::114
```

```cpp
@codesnippet
../../../tutorials/ode_advancer.cc
117:135
```

```cpp
@codesnippet
../../../tutorials/ode_advancer.cc
138:165
```

## advance_n_times_and_observe

```cpp
@codesnippet
../../../tutorials/ode_advancer.cc
182:197
```

Similarly works for the other cases accepting the time step setter, etc.


## advance_to_target_time

This always needs a functor to set the time step.

```cpp
@codesnippet
../../../tutorials/ode_advancer.cc
200:226
```

## advance_to_target_time_and_observe

This is like above, but you also need to pass an obsever.
