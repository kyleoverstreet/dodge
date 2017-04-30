class Input {
    public:
        int loc[2];
        int size;
        char player1[100];
        char player2[100];
        Input () {
            loc[0] = 0;
            loc[1] = 0;
            player1[0] = '\0';
            player2[0] = '\0';
            size = 32;
        }
};
