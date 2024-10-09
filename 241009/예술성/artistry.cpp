#include <iostream>
#include <vector>
#include <queue>

int N;
int tmpGrid[30][30];
int grid[30][30];
bool visited[30][30];

int dx[4] = { -1,1,0,0 };
int dy[4] = { 0,0,-1,1 };

std::vector<std::vector<std::pair<int, int>>> groups;
int combination = 0;

void bfs(int ix, int iy, int currNum) {
	std::vector <std::pair<int, int>> coords;
	std::queue<std::pair<int, int>>q;
	q.push({ ix,iy });
	coords.push_back({ ix,iy });
	visited[ix][iy] = true;

	while (!q.empty()) {
		int cx = q.front().first;
		int cy = q.front().second;
		q.pop();

		for (int k = 0; k < 4; k++) {
			int nx = cx + dx[k];
			int ny = cy + dy[k];

			if (nx < 0 || ny < 0 || nx >= N || ny >= N)
				continue;
			if (visited[nx][ny])
				continue;
			if (grid[nx][ny] != currNum)
				continue;
			
			q.push({ nx,ny });
			coords.push_back({ nx,ny });
			visited[nx][ny] = true;
		}
	}

	groups.push_back(coords);
}

void grouping() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (!visited[i][j]) {
				int currNum = grid[i][j];
				bfs(i, j, currNum);
			}
		}
	}
}

void scoring() {
	int groupSize = groups.size();
	int connectedLine;
	for (int i = 0; i < groupSize - 1; i++) {
		std::vector<std::pair<int, int>> groupA = groups[i];
		int numA = grid[groupA[0].first][groupA[0].second];

		for (int j = i + 1; j < groupSize; j++) {
			connectedLine = 0;
			std::vector<std::pair<int, int>> groupB = groups[j];
			int numB = grid[groupB[0].first][groupB[0].second];

			int blankA = groupA.size();
			int blankB = groupB.size();

			// connected line
			for (std::pair<int, int> coordA : groupA) {
				int cx = coordA.first;
				int cy = coordA.second;

				for (int k = 0; k < 4; k++) {
					int nx = cx + dx[k];
					int ny = cy + dy[k];

					if (nx < 0 || ny < 0 || nx >= N || ny >= N)
						continue;
					if (grid[nx][ny] == numA) // self
						continue;

					bool matched = false;
					for (std::pair<int, int> coordB : groupB) {
						if (nx == coordB.first && ny == coordB.second)
							matched = true;
					}
					if (matched)
						connectedLine++;
				}
			}

			int currComb = (blankA + blankB) * numA * numB * connectedLine;
			combination += currComb;
		}
	}
}

void rotate() {
	// cross 90
	int middle = N / 2;
	for (int i = 0; i < N; i++) {
		tmpGrid[middle][i] = grid[i][middle];
	}
	for (int i = 0; i < N; i++) {
		tmpGrid[i][middle] = grid[middle][N - i - 1];
	}
	 
	// remainder 90
	int size = N / 2;
	std::vector<std::pair<int, int>> startCoords = { {0,0}, {0,N / 2 + 1},{N / 2 + 1,0}, {N / 2 + 1,N / 2 + 1} };
	for (std::pair<int, int> startCood : startCoords) {
		int sx = startCood.first;
		int sy = startCood.second;

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				tmpGrid[sx + i][sy + j] = grid[sx + size - j - 1][sy + i];
			}
		}
	}	

	// update
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			grid[i][j] = tmpGrid[i][j];
		}
	}
}

void initialize() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			visited[i][j] = false;
		}
	}

	groups.clear();
}

int main() {
	std::cin >> N;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cin >> grid[i][j];
		}
	}

	// copy
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			tmpGrid[i][j] = grid[i][j];
		}
	}
	

	// init
	grouping();
	scoring();

	// rotate 1
	initialize();
	rotate();

	grouping();
	scoring();

	// rotate 2
	initialize();
	rotate();

	grouping();
	scoring();
	
	// rotate 3
	initialize();
	rotate();

	grouping();
	scoring();

	std::cout << combination << std::endl;

	return 0;
}