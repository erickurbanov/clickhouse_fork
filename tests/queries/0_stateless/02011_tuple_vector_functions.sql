SELECT tupleHammingDistance(tuple(1), tuple(1));
SELECT tupleHammingDistance(tuple(1, 3), tuple(1, 2));

SELECT tuple(1, 2) + tuple(3, 4) * tuple(5, 1) - tuple(6, 3);
SELECT vectorDifference(tuplePlus(tuple(1, 2), tuple(3, 4)), tuple(5, 6));
SELECT tupleMinus(vectorSum(tupleMultiply(tuple(1, 2), tuple(3, 4)), tuple(5, 6)), tuple(31, 41));
