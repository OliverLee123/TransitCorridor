# TransitCorridor
Transit Corridor is a profile of human mobility pattern at a group level.

The innovation of this algorithm is regarding stops(bus stops and subway stops) as the basic clustering unit instead of traffic line segments.Besides,in order to quantify the accumulated possession of public transport resources,we came up with a formula to calculate it and define it as load(in the program)or accumulated shared-flow(in the literature).Then take shared-flow and accumulated shared-flow both as the principles to cluster stops and then extract travel corridor.

Upload it as the summary of the previous nearly 9-months work that beginning with extracting individual trip from the raw data of Shenzhen SCD.The first time I use C++ to complete a algorithm.Although the code is bad,it makes some sense.Just take it as the little forwarding step.

Details in literature: Identifying Primary Public Transit Corridors Using Multi-source Big Transit Data.(2018)
