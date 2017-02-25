#!/bin/sh

BENCH="java -jar normalize-bench/target/benchmarks.jar -wi 5 -i 5 -f 1"

$BENCH -bm thrpt -rf csv -rff results-thrpt.csv -tu s
$BENCH -bm avgt -rf csv -rff results-avgt.csv -tu ns

