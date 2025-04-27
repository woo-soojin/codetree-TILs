#include <iostream>
using namespace std;

int N, M, K, C;
int grid[21][21]; // 0: empty, -101: wall, 1~100: tree
int tmpGrid[21][21];

int gx[4] = { -1,1,0,0 };
int gy[4] = { 0,0,-1,1 };

int ans = 0;

void copyGrid() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			grid[i][j] = tmpGrid[i][j];
		}
	}
}

void grow() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (grid[i][j] > 0) {
				int tree = 0;
				for (int d = 0; d < 4; d++) {
					int nx = i + gx[d];
					int ny = j + gy[d];

					if (nx < 0 || ny < 0 || nx >= N || ny >= N)
						continue;
					if (grid[nx][ny] > 0)
						tree++;
				}

				tmpGrid[i][j] += tree;
			}
		}
	}

	copyGrid();
}

void diffusion() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (grid[i][j] > 0) {
				int emptyCnt = 0;
				for (int d = 0; d < 4; d++) {
					int nx = i + gx[d];
					int ny = j + gy[d];

					if (nx < 0 || ny < 0 || nx >= N || ny >= N)
						continue;
					if (grid[nx][ny] == 0)
						emptyCnt++;
				}

				if (emptyCnt > 0) {
					int diffTree = grid[i][j] / emptyCnt;

					for (int d = 0; d < 4; d++) {
						int nx = i + gx[d];
						int ny = j + gy[d];

						if (nx < 0 || ny < 0 || nx >= N || ny >= N)
							continue;
						if (grid[nx][ny] == 0)
							tmpGrid[nx][ny] += diffTree;
					}
				}
			}
		}
	}

	copyGrid();
}

int rx[4] = { -1,-1,1,1 };
int ry[4] = { -1,1,-1,1 };
pair<int,int> pickKillerPos() {
	int maxRemoval = 0;
	int mx = -1;
	int my = -1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (grid[i][j] > 0) {
				int removal = grid[i][j];

				for (int d = 0; d < 4; d++) {
					for (int k = 1; k <= K; k++) {
						int nx = i + rx[d] * k;
						int ny = j + ry[d] * k;

						if (nx < 0 || ny < 0 || nx >= N || ny >= N)
							break;
						if (grid[nx][ny] <= 0)
							break;
						if (grid[nx][ny] > 0) {
							removal += grid[nx][ny];
						}
					}
				}

				if (maxRemoval < removal) {
					maxRemoval = removal;
					mx = i;
					my = j;
				}
			}	
		}
	}

	ans += maxRemoval;

	return { mx,my };
}

void kill(int x, int y) {
	tmpGrid[x][y] = (C + 1) * (-1);

	for (int d = 0; d < 4; d++) {
		for (int k = 1; k <= K; k++) {
			int nx = x + rx[d] * k;
			int ny = y + ry[d] * k;

			if (nx < 0 || ny < 0 || nx >= N || ny >= N)
				break;
			if (grid[nx][ny] <= 0) {
				if (grid[nx][ny] != -101)
					tmpGrid[nx][ny] = (C + 1) * (-1);
				break;
			}
				
			if (grid[nx][ny] > 0)
				tmpGrid[nx][ny] = (C + 1) * (-1);
		}
	}

	copyGrid();
}

void cleaner() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (grid[i][j] < 0 && grid[i][j] != -101) {
				tmpGrid[i][j]++;
			}
		}
	}

	copyGrid();
}

int main() {
	cin >> N >> M >> K >> C;

	int input;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> input;
			if (input == -1)
				input = -101;

			grid[i][j] = input;
			tmpGrid[i][j] = input;
		}
	}

	for (int m = 0; m < M; m++) {
		grow();

		diffusion();
		copyGrid();

		pair<int, int> killPos = pickKillerPos();
		kill(killPos.first, killPos.second);

		cleaner();
	}



	cout << ans << endl;

	return 0;
}