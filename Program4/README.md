Code contains implementation of various page eviction policies in an OS including optimal, clocking, randomizing, FIFO, and least recently used.
Performance of policies is measured by using 3 different workloads: No locality, looping, and 80-20.

Graphing of results can be done by using script found in plot_hit_rates.plt and running the terminal command:

      gnuplot -e "title='sample'" -e "input_filename='sample_data.csv'" plot_hit_rates.plt > sample_plot.png
      
      where 'sample_data.csv' is a file that contains the recorded results of each policy under a specified workload
          and 'sample_plot.png' is the desired title of the resulting graph representation.
