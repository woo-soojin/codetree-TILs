#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int N, M, K;
int grid[10][10];
int attackLog[10][10]; // TODO
bool visited[10][10]; // TODO init
bool isAttacked[10][10];

pair<int,int> findAttacker() {
	int x, y;
	int minPower = 5001;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (grid[i][j] == 0)
				continue;

			if (grid[i][j] < minPower) {
				minPower = grid[i][j];
				x = i;
				y = j;
			}
			else if (grid[i][j] == minPower) {
				if (attackLog[i][j] > attackLog[x][y]) {
					minPower = grid[i][j];
					x = i;
					y = j;
				}
				else if (attackLog[i][j] == attackLog[x][y]) {
					if ((i + j) > (x + y)) {
						minPower = grid[i][j];
						x = i;
						y = j;
					}
					else if ((i + j) == (x + y)) {
						if (j > y) {
							minPower = grid[i][j];
							x = i;
							y = j;
						}
					}
				}
			}
		}
	}

	return { x,y };
}

pair<int, int> findTarget(int attacker_x=-1, int attacker_y=-1) {
	int x, y;
	int maxPower = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (grid[i][j] == 0)
				continue;
			if (i == attacker_x && j == attacker_y)
				continue;

			if (grid[i][j] > maxPower) {
				maxPower = grid[i][j];
				x = i;
				y = j;
			}
			else if (grid[i][j] == maxPower) {
				if (attackLog[i][j] < attackLog[x][y]) {
					maxPower = grid[i][j];
					x = i;
					y = j;
				}
				else if (attackLog[i][j] == attackLog[x][y]) {
					if ((i + j) < (x + y)) {
						maxPower = grid[i][j];
						x = i;
						y = j;
					}
					else if ((i + j) == (x + y)) {
						if (j < y) {
							maxPower = grid[i][j];
							x = i;
							y = j;
						}
					}
				}
			}
		}
	}

	return { x,y };
}

// 우 하 좌 상
int dx[4] = { 0,1,0,-1 };
int dy[4] = { 1,0,-1,0 };
bool laserAttack(int attacker_x, int attacker_y, int target_x, int target_y) {
	pair<int, int> tracePrev[10][10]; // TODO init
	queue<pair<int, int>>q;
	q.push({ attacker_x,attacker_y });
	visited[attacker_x][attacker_y] = true;

	// find laser attack trace
	bool isLaserAttack = false;
	while (!q.empty()) {
		int cx = q.front().first;
		int cy = q.front().second;

		if (cx == target_x && cy == target_y) {
			isLaserAttack = true;
			break;
		}

		for (int d = 0; d < 4; d++) {
			int nx = (cx + dx[d] + N) % N; // TODO
			int ny = (cy + dy[d] + M) % M;

			//if (nx < 0 || ny < 0 || nx >= N || ny >= N)
			//	continue;

			if (grid[nx][ny] == 0)
				continue;
			if (visited[nx][ny])
				continue;
			
			q.push({ nx,ny });
			visited[nx][ny] = true;
			tracePrev[nx][ny] = { cx,cy };
		}

		q.pop();
	}

	if (isLaserAttack) {
		// laser attack
		int attackPower = grid[attacker_x][attacker_y];
		int px = target_x;
		int py = target_y;

		while (1) {
			if (px == attacker_x && py == attacker_y)
				break;
			if (grid[px][py] == 0)
				continue;
			if (px == target_x && py == target_y)
				grid[px][py] = max(0, grid[px][py] - attackPower);
			else
				grid[px][py] = max(0, grid[px][py] - attackPower / 2); // else

			isAttacked[px][py] = true; // TODO

			int x = px;
			int y = py;
			px = tracePrev[x][y].first;
			py = tracePrev[x][y].second;
		}
	}

	return isLaserAttack;
}

int rx[9] = { -1,-1,-1,0,0,0,1,1,1, };
int ry[9] = { -1,0,1,-1,0,1,-1,0,1 };
void bombAttack(int attacker_x, int attacker_y, int target_x, int target_y) {
	int attackPower = grid[attacker_x][attacker_y];

	for (int d = 0; d < 9; d++) {
		int nx = (target_x + rx[d] + N) % N;
		int ny = (target_y + ry[d] + M) % M;

		if (nx == attacker_x && ny == attacker_y)
			continue;
		if (grid[nx][ny] == 0)
			continue;
		if (nx == target_x && ny == target_y)
			grid[nx][ny] = max(0, grid[nx][ny] - attackPower);
		else
			grid[nx][ny] = max(0, grid[nx][ny] - attackPower / 2);

		isAttacked[nx][ny] = true;
	}
}

bool isStop() {
	int cnt = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (grid[i][j] > 0)
				cnt++;
		}
	}

	if (cnt == 1)
		return true;

	return false;
}

void maintenance(int attacker_x, int attacker_y, int target_x, int target_y) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (grid[i][j] == 0)
				continue;
			if (isAttacked[i][j] == true)
				continue;
			if (i == attacker_x && j == attacker_y)
				continue;
			if (i == target_x && j == target_y)
				continue;

			grid[i][j]++; // maintenance
		}
	}
}

void initialize() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			visited[i][j] = false;
			isAttacked[i][j] = false;
		}
	}
}

int main() {
	cin >> N >> M >> K;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> grid[i][j];
		}
	}

	for (int k = 1; k <= K; k++) {
		initialize();
		if (isStop())
			break;

		pair<int, int> attacker = findAttacker();
		grid[attacker.first][attacker.second] += (N + M);

		pair<int, int> target = findTarget(attacker.first, attacker.second);

		bool isLaserAttack = laserAttack(attacker.first, attacker.second, target.first, target.second);
		if (!isLaserAttack)
			bombAttack(attacker.first, attacker.second, target.first, target.second);

		attackLog[attacker.first][attacker.second] = k; // TODO

		maintenance(attacker.first, attacker.second, target.first, target.second);
	}

	pair<int, int> target = findTarget();
	cout << grid[target.first][target.second];

	// debug
	/*cout << endl;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cout << grid[i][j] << " ";
		}
		cout << endl;
	}*/

	return 0;
}