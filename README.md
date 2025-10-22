# Einstein_Quiz

Solution for Einstein Quiz in C.

## Features

- Compile with **GCC**  or **G++** depending on the `TRACE` flag.
- Trace generates a json file that can be viewed in a chromium based browser under `about:tracing`
- For more info about the tracing module: https://github.com/hrydgard/minitrace
- Compiler can be manually set

## Output

#### Calculation time
Time to find the solution
`Calculation time: 0.056000 milliseconds`

#### Check
- True if the program checked all rules successfully

#### Solution table
```bash
House    Nationality  Color    Drink    Cigarette    Pet   
---------------------------------------------------------------
FIRST    NORWEGIAN    YELLOW   WATER    DUNHILL      CAT   
MIDDLE   BRITT        RED      MILK     PALLMALL     BIRD  
FIFTH    SWEDE        WHITE    BEER     WINFIELD     DOG   
FOURTH   GERMAN       GREEN    COFFEE   ROTHMANNS    FISH  
SECOND   DANE         BLUE     TEA      MARLBORO     HORSE 
```
#### Completion table
Shows for each house wich rules have been checked
- 1 if checked
- 0 if not

Fish should be 0

#### Print time
Time to print output

`Print time: 0.016000 milliseconds`

## Usage

### Build

By default, GCC is used.
```bash
make
```

Enable tracing (uses G++)

```bash
make TRACE=1
```

Clean build artifacts

```bash
make clean
```

Rebuild from scratch

```bash
make rebuild
```

Use gcc

```bash
make CC=gcc
```

Use g++

```bash
make CC=g++
```