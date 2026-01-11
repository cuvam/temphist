Requires a "thermal log" that can be constructed using this CRON entry (requires `lm-sensors`):

```
0 * * * * echo "$(date '+\%Y-\%m-\%d \%H:\%M:\%S') Fan:$(sensors | awk '/Processor Fan/{print $3}') Pkg:$(sensors | awk '/Package id 0/{print $4}' | tr -d '°') Core0:$(sensors | awk '/Core 0/{print $3}' | tr -d '°') Core1:$(sensors | awk '/Core 1/{print $3}' | tr -d '°')" >> /path/to/thermal.log
```

Edit /path/to/thermal.log to where you want the log to be stored, and change the line in the C code to match. Then, after a few days, you can see the hourly distribution over the day.

Example output:

```
CPU temperature distribution per hour (283 samples):
less [ .:-+%#@] more

<42 234567890123456 >56
00:     + :++@+     | 49.58C #######################
01:     .:  @:.     | 49.50C #######################
02:    ....%@.      | 48.67C #################
03:  :  +:+ @+:     | 48.50C ################
04:   - @@@-@--     | 48.17C ##############
05:    +@ :++::     | 48.08C #############
06:     @..%...     | 48.25C ###############
07:    :::++@::     | 48.75C ##################
08:    : :+@+@      | 49.00C ####################
09:      @--%.      | 48.58C #################
10:   : +@::+::     | 48.08C #############
11:    ::++@::      | 48.09C #############
12:     +++@::      | 48.18C ##############
13:  :  ::@+:+      | 48.18C ##############
14:    -@-@@- --    | 48.45C ################
15:    ::@::+: :    | 48.45C ################
16:     :+++@:   :  | 49.17C #####################
17:      ..::@  .   | 50.17C ###########################
18:       +:@++::   | 50.67C ###############################
19:       .%@-..    | 50.17C ###########################
20:       :.@:..    | 50.17C ###########################
21:    .  + @:.     | 49.42C ######################
22:    : :::@@+     | 49.67C ########################
23:      -- @--     | 49.67C ########################
```
