#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

int n, m, k;
int visited[21][21];
int grid[21][21];
int dx[4] = { 0,-1,0,1 };
int dy[4] = { 1,0,-1,0 };
std::vector<std::vector<std::pair<int, int>>> teams;
std::vector<std::pair<int, int>> teamCoord;
int score = 0;

void teaming(int x, int y, int teamNum) {
    std::queue<std::pair<int, int>> q;
    q.push(std::make_pair(x, y));
    grid[x][y] = teamNum;
    teamCoord.push_back(std::make_pair(x, y));
    visited[x][y] = true;

    while (!q.empty()) {
        int _x = q.front().first;
        int _y = q.front().second;
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nx = _x + dx[i];
            int ny = _y + dy[i];

            if (nx < 0 || ny < 0 || nx >= n || ny >= n)
                continue;
            if (visited[nx][ny])
                continue;
            if (grid[nx][ny] == 0 || grid[nx][ny] == 4)
                continue;
            
            q.push(std::make_pair(nx, ny));
            grid[nx][ny] = teamNum;
            teamCoord.push_back(std::make_pair(nx, ny));
            visited[nx][ny] = true;
        }
    }

    teams.push_back(teamCoord);
    teamCoord.clear();
}

void move() {
    for (auto &team : teams) {
        int headX = team[0].first;
        int headY = team[0].second;
        int currTeam = grid[headX][headY];

        // move head
        for (int i = 0; i < 4; i++) {
            int nx = headX + dx[i];
            int ny = headY + dy[i];

            if (nx < 0 || ny < 0 || nx >= n || ny >= n)
                continue;
            if (grid[nx][ny] == 4) {
                grid[nx][ny] = currTeam;
                team.insert(team.begin(), std::make_pair(nx, ny));
            }
        }

        // move tail
        int teamSize = team.size();
        int tailX = team[teamSize - 1].first;
        int tailY = team[teamSize - 1].second;
        grid[tailX][tailY] = 4;
        team.pop_back();
    }
}

void throwBall(int round) {
    int sx, sy;
    int direction = (round / n) % 4;

    // init
    if (direction == 0) {
        sx = round % n;
        sy = 0;
    }
    else if (direction == 1) {
        sx = n - 1;
        sy = round % n;
    }
    else if (direction == 2) {
        sx = n - 1 - (round % n);
        sy = n - 1;
    }
    else {
        sx = 0;
        sy = n - 1 - (round % n);
    }
    
    int idx;
    for (int i = 0; i < n; i++) {
        if (sx < 0 || sy < 0 || sx >= n || sy >= n)
            continue;
        if (grid[sx][sy] > 4) {
            int teamNum = grid[sx][sy];
            int teamIdx = teamNum - 5;
            std::vector<std::pair<int, int>> currVec = teams[teamIdx];
            auto iterator = std::find(currVec.begin(), currVec.end(), std::make_pair(sx,sy));
            idx = distance(currVec.begin(), iterator);
            score += (idx + 1) * (idx + 1);

            reverse(teams[teamIdx].begin(), teams[teamIdx].end()); // reverse
            break;
        }
        sx = sx + dx[direction];
        sy = sy + dy[direction];
    }
}

int main() {
    std::cin >> n >> m >> k;

    // init
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> grid[i][j];
        }
    }

    for(int i=0; i<k; i++){
        int teamNum = 5;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (!visited[i][j] && grid[i][j] == 1) { // if head
                    teaming(i, j, teamNum);
                }
            }
            teamNum++;
        }
        move();
        throwBall(i);
    }
    
    std::cout << score;
}