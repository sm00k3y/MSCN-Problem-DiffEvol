#pragma once

// entity sizes
#define DEFAULT_ENTITY_SIZE 1
#define DEFAULT_ENTITY_INITIAL_VAL -1

// reading from file
#define MAX_LINE_LEN 2000

// RandomSearch
#define NUMBER_OF_TRIES 1'000'000
#define TRIES_UNTIL_FALLBACK 1'000'000
#define SECONDS_TILL_STOP 5.0

// DiffEvol
#define DEFAULT_POPULATION_SIZE 200
#define DEFAULT_NUMBER_OF_ITERATIONS 1000
#define CROSS_PROB 0.33
#define DIFF_WEIGHT 0.33

// random ranges
#define DEFAULT_RANDOM_MIN_VAL 0
#define DEFAULT_RANDOM_MAX_VAL 500

// contract prices
#define MIN_CONTRACT_PRICE_VAL 100
#define MAX_CONTRACT_PRICE_VAL 1000

// capacities
#define MIN_CAPACITY_AMOUNT 1000
#define MAX_CAPACITY_AMOUNT 7000

// shop incomes
#define MIN_SHOP_INCOME 50
#define MAX_SHOP_INCOME 500

// costs
#define MIN_COST_VAL 0
#define MAX_COST_VAL 100

// product quantities
#define MIN_PRODUCT_QTY 0
#define MAX_PRODUCT_QTY 400

//repairing solution
#define DEFAULT_SUBTRACTION_VAL 10
#define MAX_TIMES_TILL_FALLBACK 100