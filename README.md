Comparing Bayesian Filtering Techniques 
======================

The code for Nao robot is available at: https://github.com/AbheekG/nao

##### Compile & Run
The source code is inside "src" directory. To compile, go inside "src" and make.
~~~
cd src
make
~~~

To run:
~~~
./filter
~~~

##### Simulation and Statistical Data

The input data required for each test, and also the statistical data generated after the simulation is stored in `src/data/test*` (Replace \* with individual test number).

To generate the data for running the simulation, and to create plots after the simulation Octave (or Matlab) is used. To install in ubuntu:
~~~
sudo apt install octave
~~~

Inside every test directory, there is an Octave script `gen.m` to generate the data for the test. Run:
~~~
octave-cli gen.m
~~~
This will generate:
- `meta.csv`: stores metadata for the test.
- `motion.csv`: stores motion measurements.
- `sensor.csv`: stores sensor measurements.

After the test is run, the following files are generated in the same test directory:
- `error-cdf.csv`: L2-error of CDF for each method by time step.
- `error-state-mean.csv`: Error in mean state for each method by time step.
- `error-state-mode.csv`: Error in mode state for each method by time step.
- `pdf-*algorithm*.csv`: PDF for *algorithm* for each timestep.
- `cdf-*algorithm*.csv`: CDF for *algorithm* for each timestep.

The scripts can help generate plots:
- `plot_data.m`: Create plots of PDF/CDF for 1-D tests.
- `plot_data2.m`: Create surface plots of PDF/CDF plots for 2-D tests.

##### Files/Folders
In this project, the "Eigen" (`src/Eigen`) and "MiniDNN" (`src/MiniDNN`, `https://github.com/yixuan/MiniDNN/`) libraries have been used. The code for each method is in the corresponding class and file as mentioned in the report, for example, dynamic grid filter in `DynamicGridFilter.hh` and `DynamicGridFilter.cc`. The base class is in `filter{.cc\.hh}`. All other files have code corresponding to their meaning.

##### Report and Presentation
Report in folder "report", and presentation in folder "presentation". The localization code and python behavior ported to Nao is in "nao" folder.
