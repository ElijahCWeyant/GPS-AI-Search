# GPS-AI-Search
#Made by Elijah Weyant and Christopher Jacobs for CSIT 441: Analysis/Design of Algorithms at SUNY Fredonia under Professor Ziya Arnavut

#This was made for the purpose of testing the efficiencies of different search algorithms with different heuristics

#the latest CMay17.cpp file written in C++ in the Visual Studio IDE, this file uses the roads.csv and inters.csv to determine the map
#currently, there are no curved streets for the sake of the simplified versions of search algorithms we used

#the roads.csv uses the first two number as (X,Y) coordinates of the beginning point of the street, and the second set as coordinates of the end point, while the name next to them is just the label of the street to be used as output for directions
#the inters.csv names the intersecting streets first for directional purposes, and the specific (X,Y) coordinate afterwards to determine the coordinates where they meet for determining whether there should be a turn at this point
