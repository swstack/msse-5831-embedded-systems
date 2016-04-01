# Lab 1: Counters, Timers, Scheduling

This lab is an introduction to timers, counters and crude scheduling.

### System Tasks

The system is responsible for executing tasks at a certain frequency, the following is the definition of each task:

TODO

### Experiments

We will introduce "experiments" while the system is operating which will affect the behavior of the system timing and scheduling. The experiments are defined as follows:

TODO

# Results

Each result was recorded after roughly 30 seconds of program execution.

### Default, no experiments running

```
Stats:
Green LED       : Completions (296) Misses (0)
Yellow LED      : Completions (294) Misses (0)
Red LED         : Completions (249) Misses (30)
Jitter LED      : Completions (815) Misses (362)
Hough Transform : Completions (294) Misses (0)
```

### Experiment 1

```
TODO
```

### Experiment 2

```
Stats:
Green LED       : Completions (294) Misses (0)
Yellow LED      : Completions (292) Misses (0)
Red LED         : Completions (208) Misses (66)
Jitter LED      : Completions (852) Misses (319)
Hough Transform : Completions (292) Misses (0)
```

**Note:** Twice as many misses for Red LED, but fewer for Jitter oddly enough.

### Experiment 3

```
Stats:
Green LED       : Completions (292) Misses (0)
Yellow LED      : Completions (290) Misses (0)
Red LED         : Completions (220) Misses (47)
Jitter LED      : Completions (684) Misses (477)
Hough Transform : Completions (290) Misses (0)
```

### Experiment 4

```
Stats:
Green LED       : Completions (294) Misses (0)
Yellow LED      : Completions (293) Misses (0)
Red LED         : Completions (185) Misses (82)
Jitter LED      : Completions (756) Misses (417)
Hough Transform : Completions (293) Misses (0)
```

### Experiment 5

```
Stats:
Green LED       : Completions (298) Misses (0)
Yellow LED      : Completions (297) Misses (0)
Red LED         : Completions (200) Misses (62)
Jitter LED      : Completions (533) Misses (657)
Hough Transform : Completions (297) Misses (0)
```

### Experiment 6

```
Stats:
Green LED       : Completions (292) Misses (0)
Yellow LED      : Completions (145) Misses (144)
Red LED         : Completions (0) Misses (145)
Jitter LED      : Completions (486) Misses (676)
Hough Transform : Completions (145) Misses (144)
```

### Experiment 7

```
Stats:
Green LED       : Completions (314) Misses (0)
Yellow LED      : Completions (156) Misses (156)
Red LED         : Completions (0) Misses (155)
Jitter LED      : Completions (372) Misses (877)
Hough Transform : Completions (228) Misses (83)
```
