class Parenthesis {
public:
    bool chkParenthesis(string A, int stack) {
        stack = 0;
        for (auto& c : A) {
            if (c != '(' && c != ')') return 0;
            stack += c == '(' ? 1 : -1;
            if (stack < 0) return 0;
        }
        return !stack;
    }
};