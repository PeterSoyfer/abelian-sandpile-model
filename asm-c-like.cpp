#include<iostream>
#include<iomanip>
#include<vector>
#include<algorithm>

#define N 4

int max_width(const std::vector<int>& v)
{
	int result = 0, tmp_width = 0;
	for (int entry : v)
	{
		tmp_width = std::to_string(entry).length();
		if (tmp_width > result)
			result = tmp_width;
	}
	return result;
}

void print_vector(const std::vector<int>& v, const int width)
{
	//std::cout << " (";
	for (int i = 0; i < v.size(); ++i)
		std::cout << std::setw(width) << v[i] << ' ';
	//std::cout << std::setw(width) << v[v.size() - 1]; //<< std::endl;
}


void print_matrix(const std::vector<std::vector<int>>& matrix)
{
	int width = 0, tmp = 0;
		
	//determine max width
	for (const auto& row : matrix)
	{
		tmp = max_width(row);
		if (tmp > width)
			width = tmp;
	}
		
	width += 2; //spaces for padding
	
	for (const auto& row : matrix)
	{
		print_vector(row, width);
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


std::vector<int> matrix_vector_multiplication(const std::vector<std::vector<int>>& matrix, const std::vector<int>& vector)
{
	if (matrix.size() != N)
		std::cerr << "\nMatrix has inappropriate number of rows!\n";
	
	else if (vector.size() != N)
		std::cerr << "\nVector has inappropriate number of entries!\n";
	
	std::vector<int> result(N);
	
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			result[i] += matrix[i][j] * vector[j];
	
	return result;
}


std::vector<std::vector<int>> init_toppling_matrix()
{
	std::vector<std::vector<int>> toppling_matrix(N, std::vector<int>(N));
	
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
		{
			if (i == j)
				toppling_matrix[i][j] = 2;
			
			else if (abs(i-j) == 1)
				toppling_matrix[i][j] = -1;
		}
	
	return toppling_matrix;
}


std::vector<std::vector<int>> init_recurr_config_set()
{
	std::vector<std::vector<int>> R(N + 1, std::vector<int>(N));
	std::vector<int> R_row(N);
	
	std::fill(R_row.begin(), R_row.end(), 2);
	
	for (int i = 0; i < N; ++i)
	{
		--R_row[i];
		R[i] = R_row;
		++R_row[i];
	}
	
	R[N] = R_row;
	return R;
}


void check_R_sums(const std::vector<std::vector<int>>& R)
{
	int checksum;

	for (const auto& row : R)
	{
		checksum = 0;

		for (const auto& entry : row)
			checksum += entry;

		print_vector(row, max_width(row));
		std::cout << " checksum = " << checksum << std::endl;
	}
}


std::vector<int> stabilise(const std::vector<int>& v, std::vector<int>& v_topplings) //, std::vector<int>& v_stabilised
{
	std::vector<int> topplings_vector(N), v_stable(N);
	std::vector<std::vector<int>> delta(N, std::vector<int>(N));
	
	delta = init_toppling_matrix();
	v_stable = v;
	
	//recursive loop of stabilisations until max site == 2 or less
	while (*max_element(v_stable.begin(), v_stable.end()) > 2)
	{
		for (int i = 0; i < N; ++i)
		{
			if (v_stable[i] > 2)
			{
				++topplings_vector[i];
				std::transform(v_stable.begin(), v_stable.end(), delta[i].begin(), v_stable.begin(), std::minus<int>());
			}
		}
	}
	
	//std::cout << "Final stable configuration:";
	//print_vector(v_stable);
	
	v_topplings = topplings_vector;
	//std::cout << "Number of topplings vector:";
	//print_vector(v_topplings);
	//std::cout << std::endl;
	
	return v_stable;
}


void examine_recurr_config_set_stabilisations(const std::vector<std::vector<int>>& R)
{
	std::vector<int> v(N), v_stabilised(N), v_topplings(N);
	
	//implement sanity check: R has dimensions (N+1) x N
	
	//Total examination of recurrent set configurations stabilisation
	for (int i = 0; i < N + 1; ++i)
	{
		v = R[i];
		
		for(int j = N-1; j >= 0; --j)
			if (v[j] > 1)
			{
				++v[j];
				std::cout << "\nInitial configuration: ";
				print_vector(v, max_width(v));
			
				v_stabilised = stabilise(v, v_topplings);
				--v[j];
				
				std::cout << "\nFinal stable configuration: ";
				print_vector(v_stabilised, max_width(v_stabilised));
	
				std::cout << "\nNumber of topplings vector: ";
				print_vector(v_topplings, max_width(v_topplings));
				std::cout << std::endl;
			}
	}
}


std::vector<int> oplus(const std::vector<int>& u, const std::vector<int>& v)
{
	std::vector<int> result(N), toppl_v(N);
	
	//regular elementwise vector sum
	std::transform(v.begin(), v.end(), u.begin(), result.begin(), std::plus<int>());
	
	//stabilising the resulting configuration
	result = stabilise(result, toppl_v);
	
	return result;
}


void caley_table(const std::vector<std::vector<int>>& R)
{
	if (R.size() != N + 1)
		std::cerr << "\nGroup order mismatch! (R matrix has inappropriate number of rows)\n";
	
	else if (R[0].size() != N)
		std::cerr << "\nFirst element of the group has inappropriate number of entries!\n";
	
	else
	{
		int max_width = 0;
		
		//determine max width
		for (const auto& row : R)
			for (int entry : row)
			{
				int tmp_width = std::to_string(entry).length();
				if (tmp_width > max_width)
					max_width = tmp_width;
			}
		
		int column_width = max_width + 2; //spaces for padding
		int table_width = (N+1) * column_width + N+1; //N+1 columns and N+1 separators

		//printing header
		std::cout << std::setw(table_width-1) << "oplus | ";
		for (int i = 0; i < N+1; ++i)
		{
			print_vector(R[i], column_width);
			std::cout << " | ";
		}
		std::cout << std::endl;

		//printing top divider
		std::cout << std::string((N+2)*table_width, '-') << '\n';

		//printing table rows
		std::vector<int> tmp(N);
		for (int i = 0; i < N+1; ++i)
		{
			print_vector(R[i], column_width);
			std::cout << " | ";
			for(int j = 0; j < N+1; ++j)
			{
				tmp = oplus(R[i], R[j]);
				print_vector(tmp, column_width);
				std::cout << " | ";
			}
			std::cout << '\n';
			std::cout << std::string((N+2)*table_width, '-') << '\n';
		}
	}
}

/*
class AbelianSandpile
{
public:
	AbelianSandpile()
	{
		std::vector<std::vector<int>> R(N + 1, std::vector<int>(N));
		std::vector<int> R_row(N);

		std::fill(R_row.begin(), R_row.end(), 2);

		for(int i = 0; i < N; ++i)
		{
			--R_row[i];
			R[i] = R_row;
			++R_row[i];
		}

		R[N] = R_row;
	}

	

private:
	std::vector<std::vector<int>> R; //this represents the recurrent configuration set
};
*/


int main()
{
	std::vector<std::vector<int>> delta(N, std::vector<int>(N));
	std::vector<std::vector<int>> R(N+1, std::vector<int>(N));
	std::vector<int> v(N), u(N), w(N); //v_stabilised(N), v_topplings(N);
	
	delta = init_toppling_matrix();
	print_matrix(delta);
	
	R = init_recurr_config_set();
	check_R_sums(R);
	//examine_recurr_config_set_stabilisations(R);
	
	//std::cout << "\nCaley table for R:\n";
	//caley_table(R);
	
	//v = {4, 3, 2, 2, 2};
	//print_vector(v, max_width(v));
	//std::cout << std::endl;
	
	//u = stabilise(v, w);
	//print_vector(u, max_width(u));
	//std::cout << std::endl;
	
	//w = matrix_vector_multiplication(delta, v);
	//print_vector(w, max_width(w));
	//std::cout << std::endl;
	
	//std::transform(v.begin(), v.end(), u.begin(), w.begin(), std::plus<int>());
	
	/*std::cout << "Topplings vector for", print_vector(v), std::cout << "is:";*/
	//int width = std::to_string(v[0]).length() + 4;
	//print_vector(v, width);
	/*std::cout << "\n\nStabilised", print_vector(v), std::cout << "is:";
	print_vector(u);*/
	std::cout << "\n\n";
	
	return 0;
}
