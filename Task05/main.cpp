#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Eneter graph name as program argument\n";
		exit(EXIT_FAILURE);
	}

	auto filename = argv[1];
	std::ifstream inputFile(filename);

	if (!inputFile)
	{
		std::cout << "Error openning file!\n";
		exit(EXIT_FAILURE);
	}

	std::string line;

	// @tasks
	std::getline(inputFile, line);
	std::string no_tasks_str = line.substr(7, line.size() - 7);
	int no_tasks = std::stoi(no_tasks_str);

	std::vector<std::string> tasks;
	for (size_t i = 0; i < no_tasks; i++)
	{
		std::getline(inputFile, line);
		tasks.push_back(line);
	}

	// @proc
	std::getline(inputFile, line);
	std::string no_proc_str = line.substr(6, line.size() - 6);
	int no_proc = std::stoi(no_proc_str);

	std::vector<std::vector<int>> proc;
	for (size_t i = 0; i < no_proc; i++)
	{
		std::getline(inputFile, line);
		std::vector<int> row;
		std::istringstream iss(line);
		int value;

		while (iss >> value)
			row.push_back(value);

		proc.push_back(row);
	}

	// @times
	std::getline(inputFile, line);
	std::vector<std::vector<int>> times;

	for (size_t i = 0; i < no_tasks; i++)
	{
		std::getline(inputFile, line);
		std::vector<int> row;
		std::istringstream iss(line);
		int value;

		while (iss >> value)
			row.push_back(value);

		times.push_back(row);
	}

	// @cost
	std::getline(inputFile, line);
	std::vector<std::vector<int>> cost;
	for (size_t i = 0; i < no_tasks; i++)
	{
		std::getline(inputFile, line);
		std::vector<int> row;
		std::istringstream iss(line);
		int value;

		while (iss >> value)
			row.push_back(value);

		cost.push_back(row);
	}

	// @comm
	std::getline(inputFile, line);
	std::string no_comm_str = line.substr(6, line.size() - 6);
	int no_comm = std::stoi(no_comm_str);

	std::vector<std::string> comm;
	for (size_t i = 0; i < no_comm; i++)
	{
		std::getline(inputFile, line);
		comm.push_back(line);
	}

	size_t time_sum = 0;
	for (size_t i = 0; i < no_tasks; ++i)
	{
		size_t min_idx = 0;
		for (size_t j = 1; j < no_proc; ++j)
			if (times[i][j] < times[i][min_idx])
				min_idx = j;
		time_sum += times[i][min_idx];
		std::cout << "T" << i << " -> P" << min_idx+1 << "\n";
	}
	std::cout << "Time sum: " << time_sum << "\n";

	/*std::cout << "@tasks " << no_tasks << "\n";
	for (const auto& x : tasks)
		std::cout << x << "\n";
	std::cout << "@proc " << no_proc << "\n";
	for (const auto& x : proc)
	{
		for (const auto& y : x)
			std::cout << y << " ";
		std::cout << "\n";
	}
	std::cout << "@times\n";
	for (const auto& x : times)
	{
		for (const auto& y : x)
			std::cout << y << " ";
		std::cout << "\n";
	}
	std::cout << "@cost\n";
	for (const auto& x : cost)
	{
		for (const auto& y : x)
			std::cout << y << " ";
		std::cout << "\n";
	}
	std::cout << "@comm " << no_comm << "\n";
	for (const auto& x : comm)
		std::cout << x << "\n";*/

	inputFile.close();
	system("pause");
}
