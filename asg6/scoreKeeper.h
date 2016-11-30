class ScoreKeeper{
    public:
        static ScoreKeeper& getInstance();
        unsigned int getScore() const;
        void setScore(unsigned int);
        float getMultiplier() {return multiplier;}
        void setMultiplier(float x) {multiplier = x;}

    private:
        unsigned int score;
        float multiplier;
        ScoreKeeper();
        ScoreKeeper(const ScoreKeeper&);
        ScoreKeeper& operator=(const ScoreKeeper&);
};