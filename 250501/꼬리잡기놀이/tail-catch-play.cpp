#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

int N, M, K;
int grid[21][21];
bool visited[21][21];
vector<vector<pair<int, int>>> teams;

int dx[4] = { -1,1,0,0 };
int dy[4] = { 0,0,-1,1 };

int ans = 0;

void teaming(int sx, int sy) {
	vector<pair<int, int>> team;
	queue<pair<int, int>> q;
	q.push({ sx,sy });
	visited[sx][sy] = true;
	team.push_back({ sx,sy });

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
			if (grid[nx][ny] == 0 || grid[nx][ny] == 4) // empty or line
				continue;
			if (grid[cx][cy] == 1 && grid[nx][ny] == 3) // 1 2 3 순서대로 넣기위한 처리 (주의!!!)
				continue;

			q.push({ nx,ny });
			visited[nx][ny] = true;
			team.push_back({ nx,ny });
		}

		q.pop();
	}

	teams.push_back(team);
}

void moveForward() {
	for (vector<pair<int, int>>& team : teams) {
		int teamSize = team.size();

		// move tail
		int tailX = team[teamSize - 1].first;
		int tailY = team[teamSize - 1].second;
		int tailPrevX = team[teamSize - 2].first;
		int tailPrevY = team[teamSize - 2].second;

		grid[tailPrevX][tailPrevY] = 3; // tail
		grid[tailX][tailY] = 4; // tail
		team.pop_back();
		
		// move head
		int headX = team[0].first;
		int headY = team[0].second;
		grid[headX][headY] = 2;

		for (int d = 0; d < 4; d++) {
			int nx = headX + dx[d];
			int ny = headY + dy[d];

			if (nx < 0 || ny < 0 || nx >= N || ny >= N)
				continue;
			if (grid[nx][ny] == 4) { // TODO 항상 한개?
				grid[nx][ny] = 1;
				team.insert(team.begin(), { nx,ny });
			}
		}
	}
}

bool scoring(int i, int j) {
	int score = 0;
	if (grid[i][j] != 0 && grid[i][j] != 4) {
		for (vector<pair<int, int>>& team : teams) {
			int teamSize = team.size();
			for (int idx = 0; idx < teamSize; idx++) {
				int tx = team[idx].first;
				int ty = team[idx].second;

				if (tx == i && ty == j) {
					score = pow(idx + 1, 2);
					ans += score;
					reverse(team.begin(), team.end());

					return true;
				}
			}
		}
	}

	return false;
}

void throwBall(int k) {
	int round = k % (N * 4);
	int cx, cy;

	if (round < N) {
		cx = round;
		for (int j = 0; j < N; j++) {
			if (scoring(cx, j))
				return;
		}
	}
	else if (round >= N && round < 2 * N) {
		cy = round % N;
		for (int i = N - 1; i >= 0; i--) {
			if (scoring(i, cy))
				return;
		}
	}
	else if (round >= 2 * N && round < 3 * N) {
		cx = (N - 1) - round % N;
		for (int j = N - 1; j >= 0; j--) {
			if (scoring(cx, j))
				return;
		}
	}
	else if (round >= 3 * N && round < 4 * N) {
		cy = (N - 1) - round % N;
		for (int i = 0; i < N; i++) {
			if (scoring(i, cy))
				return;
		}
	}
}

int main() {
	// TODO M 사용
	cin >> N >> M >> K;
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> grid[i][j]; // 0: emtpy, 1: head, 2: remainder, 3: tail, 4: line
		}
	}

	// teaming
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (grid[i][j]==1 && !visited[i][j])
				teaming(i, j);
		}
	}

	for (int k = 0; k < K; k++) {
		moveForward();
		throwBall(k);
	}
	
	cout << ans << endl;

	return 0;
}