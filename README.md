# TransitCorridor
Transit Corridor is a profile of human mobility pattern at a group level.

The innovation of our algorithm is that we regard stops(bus stops and subway stops) as the basic clustering unit instead of traffic line segments.Besides,in order to quantify the accumulated possession of public transport resources,we came up with a formula to calculate it and define it as load(in the program)or accumulated shared-flow(in the literature).Then take shared-flow and accumulated shared-flow both as the principles to cluster stops to extract travel corridor.

Upload it as the summary of the previous nearly 9-months work that beginning with extracting individual trip from the raw data of Shenzhen Smart Card Data.The first time I use C++ to complete the algorithm.Although the code is bad,however it makes some sense,just take it as the little step that forwarding.

Details in Identifying Primary Public Transit Corridors Using Multi-source Big Transit Data.(2018)
