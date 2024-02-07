//============================================================================
// Name        : 2223S_19c_s26941.cpp
// Author      : Taha Burak Sahin
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstring>
#include <string.h>
#include <fstream>
#include <vector>
#include <string>

// Constants
const int BOARD_SIZE = 10;
const char EMPTY_CELL = 'O';
const char PLAYER_1_PAWN = 'X';
const char PLAYER_2_PAWN = '1';

// Function to initialize the game board
void initializeBoard(std::vector<std::vector<char>>& board) {
    board.resize(BOARD_SIZE, std::vector<char>(BOARD_SIZE, EMPTY_CELL));

    // Place player 1 pawns
    for (int i = 0; i < BOARD_SIZE / 2 - 1; i++) {
        for (int j = (i + 1) % 2; j < BOARD_SIZE; j += 2) {
            board[i][j] = PLAYER_1_PAWN;
        }
    }

    // Place player 2 pawns
    for (int i = BOARD_SIZE / 2 + 1; i < BOARD_SIZE; i++) {
        for (int j = (i + 1) % 2; j < BOARD_SIZE; j += 2) {
            board[i][j] = PLAYER_2_PAWN;
        }
    }
}

// Function to draw the game board
void drawBoard(const std::vector<std::vector<char>>& board) {
    int offset = BOARD_SIZE / 2;

    for (int i = 0; i < BOARD_SIZE; i++) {
        // Add spacing before each row
        for (int k = 0; k < std::abs(i - offset); k++) {
            std::cout << "  ";
        }

        // Handle each row separately
        if (i < offset) {
            // Handle the upper half of the board
            for (int j = 0; j < BOARD_SIZE - 2 * std::abs(i - offset); j++) {
                if (j % 2 == 0) {
                    std::cout << " - ";
                }
                else {
                    std::cout << " " << board[i][j / 2] << " ";
                }
            }
        }
        else if (i > offset) {
            // Handle the lower half of the board
            for (int j = 0; j < BOARD_SIZE - 2 * std::abs(i - offset); j++) {
                if (j % 2 == 0) {
                    std::cout << " - ";
                }
                else {
                    std::cout << " " << board[i][j / 2 + std::abs(i - offset)] << " ";
                }
            }
        }
        else {
            // Handle the middle row
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (j % 2 == 0) {
                    std::cout << " - ";
                }
                else {
                    std::cout << " " << board[i][j / 2] << " ";
                }
            }
        }

        std::cout << std::endl;
    }
}




// Function to save the game state to a file
void saveGame(const std::vector<std::vector<char>>& board, int player1Score, int player2Score) {
    std::ofstream file("game.txt");
    if (file.is_open()) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                file << board[i][j];
            }
            file << std::endl;
        }
        file << player1Score << std::endl;
        file << player2Score << std::endl;
        file.close();
        std::cout << "Game saved successfully." << std::endl;
    }
    else {
        std::cout << "Unable to save the game." << std::endl;
    }
}

// Function to load the game state from a file
bool loadGame(std::vector<std::vector<char>>& board, int& player1Score, int& player2Score) {
    std::ifstream file("game.txt");
    if (file.is_open()) {
        std::string line;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (std::getline(file, line)) {
                if (line.size() != BOARD_SIZE) {
                    std::cout << "Invalid game file." << std::endl;
                    file.close();
                    return false;
                }
                for (int j = 0; j < BOARD_SIZE; j++) {
                    board[i][j] = line[j];
                }
            }
            else {
                std::cout << "Invalid game file." << std::endl;
                file.close();
                return false;
            }
        }
        if (file >> player1Score >> player2Score) {
            file.close();
            std::cout << "Game loaded successfully." << std::endl;
            return true;
        }
        else {
            std::cout << "Invalid game file." << std::endl;
            file.close();
            return false;
        }
    }
    else {
        std::cout << "Unable to load the game." << std::endl;
        return false;
    }
}

// Function to check if a move is valid
bool isValidMove(const std::vector<std::vector<char>>& board, int fromRow, int fromCol, int toRow, int toCol, int currentPlayer) {
    // Check if the initial and final positions are within the board limits
    if (fromRow < 0 || fromRow >= BOARD_SIZE || fromCol < 0 || fromCol >= BOARD_SIZE ||
        toRow < 0 || toRow >= BOARD_SIZE || toCol < 0 || toCol >= BOARD_SIZE) {
        return false;
    }

    // Check if the initial position contains a pawn of the current player
    char playerPawn = (currentPlayer == 1) ? PLAYER_1_PAWN : PLAYER_2_PAWN;
    if (board[fromRow][fromCol] != playerPawn) {
        return false;
    }

    // Check if the final position is empty
    if (board[toRow][toCol] != EMPTY_CELL) {
        return false;
    }

    // Check if it's a valid diagonal move (1 or 2 steps)
    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;
    if (rowDiff == currentPlayer && (colDiff == 1 || colDiff == -1)) {
        return true;
    }
    else if (rowDiff == currentPlayer * 2 && (colDiff == 2 || colDiff == -2)) {
        int middleRow = fromRow + currentPlayer;
        int middleCol = fromCol + colDiff / 2;
        char opponentPawn = (currentPlayer == 1) ? PLAYER_2_PAWN : PLAYER_1_PAWN;
        if (board[middleRow][middleCol] == opponentPawn) {
            return true;
        }
    }

    return false;
}

// Function to make a move
bool makeMove(std::vector<std::vector<char>>& board, int fromRow, int fromCol, int toRow, int toCol, int currentPlayer) {
    if (isValidMove(board, fromRow, fromCol, toRow, toCol, currentPlayer)) {
        board[toRow][toCol] = board[fromRow][fromCol];
        board[fromRow][fromCol] = EMPTY_CELL;

        // Check if the move is a capture
        int rowDiff = toRow - fromRow;
        int colDiff = toCol - fromCol;
        if (rowDiff == currentPlayer * 2 && (colDiff == 2 || colDiff == -2)) {
            int middleRow = fromRow + currentPlayer;
            int middleCol = fromCol + colDiff / 2;
            board[middleRow][middleCol] = EMPTY_CELL;
        }

        // Check if the pawn has reached the opponent's end to become a king
        if (toRow == 0 && currentPlayer == 1) {
            board[toRow][toCol] = 'K';
        }
        else if (toRow == BOARD_SIZE - 1 && currentPlayer == -1) {
            board[toRow][toCol] = 'K';
        }

        return true;
    }

    return false;
}

// Function to count the pawns of each player
void countPawns(const std::vector<std::vector<char>>& board, int& player1Score, int& player2Score) {
    player1Score = 0;
    player2Score = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == PLAYER_1_PAWN || board[i][j] == 'K') {
                player1Score++;
            }
            else if (board[i][j] == PLAYER_2_PAWN || board[i][j] == 'k') {
                player2Score++;
            }
        }
    }
}

int main() {
    std::vector<std::vector<char>> board;
    int player1Score = 0;
    int player2Score = 0;
    int currentPlayer = 1;

    std::cout << "Welcome to Hexagon!" << std::endl;

    // Load game from file or initialize new game
    char choice;
    std::cout << "Do you want to load a saved game? (y/n): ";
    std::cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        if (!loadGame(board, player1Score, player2Score)) {
            initializeBoard(board);
            countPawns(board, player1Score, player2Score);
        }
    }
    else {
        initializeBoard(board);
        countPawns(board, player1Score, player2Score);
    }

    // Game loop
    bool isGameOver = false;
    while (!isGameOver) {
        // Draw the board
        drawBoard(board);
        std::cout << "Player 1 Score: " << player1Score << std::endl;
        std::cout << "Player 2 Score: " << player2Score << std::endl;

        // Get move from the current player
        int fromRow, fromCol, toRow, toCol;
        std::cout << "Player " << currentPlayer << ", enter your move (fromRow fromCol toRow toCol): ";
        std::cin >> fromRow >> fromCol >> toRow >> toCol;

        // Make the move
        if (makeMove(board, fromRow, fromCol, toRow, toCol, currentPlayer)) {
            // Switch to the next player
            currentPlayer = -currentPlayer;

            // Count the pawns of each player
            countPawns(board, player1Score, player2Score);

            // Check if the game is over
            if (player1Score == 0 || player2Score == 0) {
                isGameOver = true;
            }
        }
        else {
            std::cout << "Invalid move. Try again." << std::endl;
        }
    }

    // Display the final board and scores
    drawBoard(board);
    std::cout << "Player 1 Score: " << player1Score << std::endl;
    std::cout << "Player 2 Score: " << player2Score << std::endl;

    // Determine the winner
    if (player1Score > player2Score) {
        std::cout << "Player 1 wins!" << std::endl;
    }
    else if (player2Score > player1Score) {
        std::cout << "Player 2 wins!" << std::endl;
    }
    else {
        std::cout << "It's a draw!" << std::endl;
    }

    // Save the game
    std::cout << "Do you want to save the game? (y/n): ";
    std::cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        saveGame(board, player1Score, player2Score);
    }

    return 0;
}
