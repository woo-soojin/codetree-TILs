#include <iostream>
#include <queue>
using namespace std;

int N;
int grid[30][30];
int tmpGrid[30][30];
//int groupingGrid[30][30];
bool visited[30][30];
vector<vector<pair<int, int>>> groups;
int ans = 0;

int dx[4] = { -1,1,0,0 };
int dy[4] = { 0,0,-1,1 };

//void grouping(int sx, int sy, int groupNumber) {
void grouping(int sx, int sy) {
	vector<pair<int, int>> group;
	queue<pair<int, int>> q;
	int currNum = grid[sx][sy];

	q.push({ sx,sy });
	visited[sx][sy] = true;
	group.push_back({ sx,sy });
	//groupingGrid[sx][sy] = groupNumber;

	while (!q.empty()) {
		int cx = q.front().first;
		int cy = q.front().second;

		for (int d = 0; d < 4; d++) {
			int nx = cx + dx[d];
			int ny = cy + dy[d];

			if (nx < 0 || ny < 0 || nx >= N || ny >= N)
				continue;
			if (visited[nx][ny])
				continue;
			if (grid[nx][ny] == currNum) {
				q.push({ nx,ny });
				visited[nx][ny] = true;
				group.push_back({ nx,ny });
				//groupingGrid[nx][ny] = groupNumber;
			}
		}

		q.pop();
	}

	groups.push_back(group);
}

int combCalc(int numOfGridA, int numOfGridB, int groupingNumA, int groupingNumB, int connectedLine) {

	int artScore = (numOfGridA + numOfGridB) * groupingNumA * groupingNumB * connectedLine;
	return artScore;
}

void scroing() {
	// combination
	int numOfGroup = groups.size();
	for (int i = 0; i < numOfGroup - 1; i++) {
		for (int j = i + 1; j < numOfGroup; j++) {
			vector<pair<int, int>>firstGroup = groups[i];
			vector<pair<int, int>>secondGroup = groups[j];

			int firstGroupNum = grid[firstGroup[0].first][firstGroup[0].second]; // TODO
			int secondGroupNum = grid[secondGroup[0].first][secondGroup[0].second]; // TODO

			// for comparison
			//int compareGroupNum = groupingGrid[secondGroup[0].first][secondGroup[0].second];

			// connected line
			int connectedLine = 0;
			for (pair<int, int> coord : firstGroup) {
				int cx = coord.first;
				int cy = coord.second;

				for (int d = 0; d < 4; d++) {
					int nx = cx + dx[d];
					int ny = cy + dy[d];

					if (nx < 0 || ny < 0 || nx >= N || ny >= N)
						continue;
					//if (grid[nx][ny] == secondGroupNum && groupingGrid[nx][ny] == compareGroupNum)
					//	connectedLine++;

					if (grid[nx][ny] == secondGroupNum) {
						bool isExist = false;

						for (pair<int, int> nCoord : secondGroup) {
							if (nCoord.first == nx && nCoord.second == ny)
								isExist = true;
						}
						if (isExist)
							connectedLine++;
					}
				}
			}

			ans += combCalc(firstGroup.size(), secondGroup.size(), firstGroupNum, secondGroupNum, connectedLine);
		}
	}
}

void copyGrid() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			grid[i][j] = tmpGrid[i][j];
		}
	}
}

void rotate() {
	// cross (반시계 90)
	int mid = N / 2;
	for (int i = 0; i < N; i++) {
		tmpGrid[i][mid] = grid[mid][N - i - 1];
	}
	for (int i = 0; i < N; i++) {
		tmpGrid[mid][i] = grid[i][mid];
	}
	copyGrid();

	// else cross (시계 90)
	vector<pair<int, int>> sCoords = { {0,0},{0,mid + 1},{mid + 1,0},{mid + 1,mid + 1} };
	for (pair<int, int > sCoord : sCoords) {
		int sx = sCoord.first;
		int sy = sCoord.second;

		for (int i = 0; i < mid; i++) {
			for (int j = 0; j < mid; j++) {
				tmpGrid[sx + i][sy + j] = grid[sx + mid - 1 - j][sy + i];
			}
		}
	}
	copyGrid();
}

void initialize() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			visited[i][j] = false;
			//groupingGrid[i][j] = 0;
		}
	}

	groups.clear(); // TODO
}

int main() {
	cin >> N;
	int input;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> input;
			grid[i][j] = input;
			tmpGrid[i][j] = input;
		}
	}

	for (int t = 0; t < 4; t++) {
		initialize();

		if (t > 0)
			rotate();

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (!visited[i][j]) {
					//grouping(i, j, groupNumber);
					grouping(i, j);
					//groupNumber++;
				}
			}
		}
		scroing();
	}

	cout << ans << endl;

	return 0;
}