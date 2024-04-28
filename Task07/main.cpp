#include <iostream>
#include <vector>
#include <iostream>
#include "Graph.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Eneter graph name as program argument\n";
		exit(EXIT_FAILURE);
	}

	int max_time = 0;

	std::cout << "Enter max time: ";
	std::cin >> max_time;
	std::cout << std::endl;

	Graph task_graph(argv[1], max_time);
	task_graph.setFastestResources();
	task_graph.refiningAlgorithm();

	
	task_graph.displayAllocation();
	std::cout << std::endl;
	
	task_graph.displayIntervals();
	std::cout << std::endl;

	std::cout << "Critical path: " << std::endl;
	task_graph.displayCriticalPath();
	std::cout << std::endl;

	std::cout << "Final time: " << task_graph.getFinalTime() << std::endl;
	std::cout << "Final cost: " << task_graph.getFinalCost() << std::endl;
	std::cout << std::endl;

	system("pause");
}
