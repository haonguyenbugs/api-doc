# UserAgent Generator:

## How to run?

```bash
g++ -O3 -march=native -std=c++20 useragent.cpp -o useragent
```

## Args:
Generate basic for script, has 1000 UA and Mixed
```bash
./useragent
```

## Others:
1.
    **Fake args**: 
    Generate 50.000 fake User-Agent:
    ```bash
    ./useragent --n 50000 --mode fake --out fake_ua.txt
    ```
2.
    **else**:
    Generate 5000 real User-Agent
    ```bash
    ./useragent --n 5000 --mode real --out real_ua.txt
    ```
3.
    **Seed choosed**
    Generate 10.000 seeded User-Agent
    ```bash
    ./useragent --n 10000 --mode both --seed 123456 --out ua.txt
    ```