# DBSCAN
DBSCAN implementation in C++

# Important Notes
1. The clustering made in 2d dimension but can be easily changed to any other dimension you please.
2. The output may be different from time to time because of the unordered set. It doesn't mean it's wrong, as long as the points that need to be under the same cluster are classified with the same cluster ID. 

# How to run it with your own input?
I have an input file of 11 points. If you want to change the input just change in the "Set of Points" file, but be consistent: each coordinate separated with regular spacing and each point in new line. The maximum distance and the minimum amount of points you can change at the creation of the cluster class (line 130).
