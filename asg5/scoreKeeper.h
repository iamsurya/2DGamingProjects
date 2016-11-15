class ScoreKeeper{
    public:
        static ScoreKeeper& getInstance();
        unsigned int getScore() const;
        void setScore(unsigned int);

    private:
        unsigned int score;

        ScoreKeeper();
        ScoreKeeper(const ScoreKeeper&);
        ScoreKeeper& operator=(const ScoreKeeper&);
};