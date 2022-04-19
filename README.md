# connect4
connect four

### Target system: debian based

# Dependencies
Following packages:
```
build-essential libxmu-dev libxi-dev libgl-dev cmake git
```

# Build
Run the makefile targets as such:
```
$ make deps
$ make
```
Then launch by executing:
```
./connect4
```
Change the size of the transposition table (for the AI - default is 1 GB) by launching like this (omit brackets):
```
./connect4 tt=[size in megabytes]
```
