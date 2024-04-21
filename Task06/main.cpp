#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <map>
#include <set>

int time_criteria(const int& task_id, const std::vector<std::vector<int>>& times)
{
	int min = INT_MAX;
	int proc_id = 0;

	for (size_t i = 0; i < times[task_id].size(); i++)
	{
		if (times[task_id][i] < min)
		{
			min = times[task_id][i];
			proc_id = i;
		}
	}

	return proc_id;
}

int cost_citeria(const int& task_id, const std::vector<std::vector<int>>& cost)
{
	int min = INT_MAX;
	int proc_id = 0;

	for (size_t i = 0; i < cost[task_id].size(); i++)
	{
		if (cost[task_id][i] < min)
		{
			min = cost[task_id][i];
			proc_id = i;
		}
	}

	return proc_id;
}

int time_criteria_on_allocated(const int& task_id, const std::vector<std::vector<int>>& times, const std::set<int>& used_proc)
{
	int min = INT_MAX;
	int proc_id = 0;

	for (auto proc : used_proc)
	{
		if (times[task_id][proc] < min)
		{
			min = times[task_id][proc];
			proc_id = proc;
		}
	}

	return proc_id;
}

int cost_criteria_on_allocated(const int& task_id, const std::vector<std::vector<int>>& cost, const std::set<int>& used_proc)
{
	int min = INT_MAX;
	int proc_id = 0;

	for (auto proc : used_proc)
	{
		if (cost[task_id][proc] < min)
		{
			min = cost[task_id][proc];
			proc_id = proc;
		}
	}

	return proc_id;
}

int time_cost_criteria(const int& task_id, const std::vector<std::vector<int>>& cost, const std::vector<std::vector<int>>& times)
{
	int min = INT_MAX;
	int proc_id = 0;

	for (size_t i = 0; i < times[task_id].size(); i++)
	{
		int actual_cost = cost[task_id][i];
		int actual_time = times[task_id][i];
		int t_c = actual_cost * actual_time;

		if (t_c < min)
		{
			min = t_c;
			proc_id = i;
		}
	}

	return proc_id;
}

std::pair<double, double> standardDeviation(const std::vector<int>& data) 
{
	double mean = 0.0;
	for (int value : data) {
		mean += value;
	}
	mean /= data.size();

	double sumOfSquaredDifferences = 0.0;
	for (int value : data) {
		sumOfSquaredDifferences += (value - mean) * (value - mean);
	}

	double meanOfSquaredDifferences = sumOfSquaredDifferences / data.size();

	double standardDev = std::sqrt(meanOfSquaredDifferences);

	return std::make_pair(mean, standardDev);
} // returns pair of mean and standart deviation

int normalization_criteria(const int& task_id, const std::vector<std::vector<int>>& cost, const std::vector<std::vector<int>>& times, const std::vector<std::vector<int>>& proc)
{
	int a = 1;
	int b = 1;
	int y = 1;

	std::vector<int> temp_proc;
	for (size_t i = 0; i < proc.size(); i++)
		temp_proc.push_back(proc[i][0]);
	std::vector<int> temp_cost;

	auto proc_mean_stdDev = standardDeviation(temp_proc);
	auto cost_mean_stdDev = standardDeviation(cost[task_id]);
	auto times_mean_stdDev = standardDeviation(times[task_id]);

	int min = INT_MAX;
	int proc_id = 0;

	for (size_t i = 0; i < proc.size(); i++)
	{
		auto proc_norm = (proc[i][0] - proc_mean_stdDev.first) / proc_mean_stdDev.second;
		auto cost_norm = (cost[task_id][i] - cost_mean_stdDev.first) / cost_mean_stdDev.second;
		auto times_norm = (times[task_id][i] - times_mean_stdDev.first) / times_mean_stdDev.second;

		int n_c = (a * proc_norm) + (b * cost_norm) + (y * times_norm);

		if (n_c < min)
		{
			min = n_c;
			proc_id = i;
		}
	}

	return proc_id;
}

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

	inputFile.close();

	/*
	std::cout << "@tasks " << no_tasks << "\n";
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
		std::cout << x << "\n";
	*/

	std::set<int> used_proc;
	std::map<int, int> selected_proc;

	for (size_t task_id = 0; task_id < tasks.size(); task_id++)
	{
		std::map<int, int> criteria;
		int norm_c = normalization_criteria(task_id, cost, times, proc); // 200
		int time_cost_c = time_cost_criteria(task_id, cost, times); // 100
		int time_c = time_criteria(task_id, times); // 25
		int time_allocated_c = time_criteria_on_allocated(task_id, times, used_proc); // 25
		int cost_c = cost_citeria(task_id, cost); // 25
		int cost_allocated_c = cost_criteria_on_allocated(task_id, cost, used_proc); // 25 

		criteria[norm_c] += 200;
		criteria[time_cost_c] += 100;
		criteria[time_c] += 25;
		criteria[time_allocated_c] += 25;
		criteria[cost_c] += 25;
		criteria[cost_allocated_c] += 25;

		int max = INT_MIN;
		int actual_selected_proc = 0;
		for (auto x : criteria)
		{
			if (x.second > max)
			{
				max = x.second;
				actual_selected_proc = x.first;
			}
		}

		selected_proc[task_id] = actual_selected_proc;
		used_proc.insert(actual_selected_proc);
	}

	for (auto x : selected_proc)
		std::cout << x.first << "\t->\t" << x.second << std::endl;

	system("pause");
}
