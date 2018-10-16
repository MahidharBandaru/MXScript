# MXScript
MXScript is a scripting language aimed at simplicity. The development is ongoing, only certain parts of the interpreter have been written.

To build MX follow these steps:
1. Clone this repository.
2. Enter the cloned directory and do the following

```
mkdir build
cd build
cmake ..
make
cd ..
```

3. MX (path bin/MX) is the target executable. To run programs enter:
```
bin/MX <file path relative to the current working directory>
```

4. Sample functions have been written in Examples/test.mx