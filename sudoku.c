// sudoku.c
// どのようなプログラムか
// 数独パズルを解くためのプログラムで、バックトラッキングアルゴリズムを使用しており、再帰呼び出し関数で実現されている。
// 数独パズルを解くためのプログラム
// バックトラッキングアルゴリズムを使用しており、再帰呼び出し関数で実現されている。

#include <stdbool.h>
#include <stdio.h>

#define N 9  // 縦横のマス目の大きさ

long callCount = 0;  // solveSudoku関数の呼出し回数

// 関数のプロトタイプ宣言
void readGrid(int grid[N][N]);
void printGrid(int grid[N][N]);
bool solveSudoku(int grid[N][N], int row, int col);
bool check(int grid[N][N], int row, int col, int num);

// メイン
int main(void)
{
    int grid[N][N];

    readGrid(grid);   // データの読み込み
    printGrid(grid);  // 解く前の状態を表示

    if (solveSudoku(grid, 0, 0)) {  // 解く（左上からスタート）
        printf("解が見つかりました！\n");
        printGrid(grid);  // 解いた後の状態を表示
    } else {
        printf("解が見つかりません\n");
    }
    printf("call count = %ld\n", callCount);

    return 0;
}

// 標準入力から数独のデータを読み込む（9x9の数値データ）
void readGrid(int grid[N][N])
{
    int c;
    int x, y;

    // #文字以降はコメントとして無視する処理
    if ((c = getc(stdin)) == '#') {
        // #が来たら行末まで読み飛ばす
        while (getc(stdin) != '\n');
    } else {
        // #でなかったら文字を戻す
        ungetc(c, stdin);
    }

    // 数独の9x9のデータを読み込む
    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++) {
            scanf("%d", &grid[y][x]);
        }
    }
}

// 数独のグリッドとデータを表示する関数
void printGrid(int grid[N][N])
{
    int row, col;

    // 罫線の長さ：数字の列(N*2) + 縦棒の分(N/3*2) + 右端の縦棒(1) = 25（N=9のとき）
    int lineLen = N * 2 + (N / 3) * 2 + 1;

    for (row = 0; row <= N; row++) {
        // 3行ごとに横の罫線を表示
        if (row % 3 == 0) {
            if (row == 0 || row == N) {
                // 一番上と一番下の罫線は縦棒なし
                for (col = 0; col < lineLen; col++) {
                    printf("-");
                }
            } else {
                // 途中の罫線は両端に縦棒をつける
                printf("|");
                for (col = 0; col < lineLen - 2; col++) {
                    printf("-");
                }
                printf("|");
            }
            printf("\n");
        }

        // 最後の罫線を表示したら終了
        if (row == N) {
            break;
        }

        // 数字の行を表示
        for (col = 0; col < N; col++) {
            if (col % 3 == 0) {
                printf("| ");
            }

            if (grid[row][col] == 0) {
                printf(". ");
            } else {
                printf("%d ", grid[row][col]);
            }
        }
        printf("|\n");
    }
}

// 数独を解くための再帰関数（バックトラッキング）
bool solveSudoku(int grid[N][N], int row, int col)
{
    int num;

    // 関数が呼ばれた回数を増やす
    callCount++;

    // 右下まで検索したか
    if (row == N - 1 && col == N) {
        return true;  // 解が見つかった
    }

    // 右端に来たら、次の行の左端に現在位置をセット
    if (col == N) {
        row++;
        col = 0;
    }

    // 現在の場所に既に数字があれば、1つ右側から検索
    if (grid[row][col] > 0) {
        return solveSudoku(grid, row, col + 1);
    }

    // 空き部分では、1～Nまでの設定可能な数字を入れてさらに検索
    for (num = 1; num <= N; num++) {
        if (check(grid, row, col, num)) {
            grid[row][col] = num;  // 仮に数字を置く
            if (solveSudoku(grid, row, col + 1)) {
                return true;  // 現在地点までを満たすものがあった
            }
        }
        grid[row][col] = 0;  // 仮に入れた数字を削除しておく
    }

    // 現状の組合せでは見つかっていないとき
    return false;
}

// 指定された位置に、指定された数字を置くことができるかをチェックする関数
bool check(int grid[N][N], int row, int col, int num)
{
    int i;
    int startRow, startCol;  // 3x3のブロックの左上の位置
    int x, y;

    // 縦と横でチェック
    for (i = 0; i < N; i++) {
        if (grid[row][i] == num || grid[i][col] == num) {
            return false;  // その数字は既にあった
        }
    }

    // 同じ3x3のブロック内でチェック
    startRow = row - row % 3;
    startCol = col - col % 3;
    for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
            if (grid[startRow + y][startCol + x] == num) {
                return false;  // その数字は既にあった
            }
        }
    }

    // ここに来れば同じ数字は見つからなかった（つまり指定された数字は使って良い）
    return true;
}
