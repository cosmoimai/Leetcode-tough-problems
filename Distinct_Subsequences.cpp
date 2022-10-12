class Solution
{
public:
    int numDistinct(string str, string sub)
    {

        int n = str.size();
        int m = sub.size();
        vector<vector<unsigned long long>> dp(m + 1, vector<unsigned long long>(n + 1, 0LL));

        for (int i = 0; i <= n; i++)
            dp[0][i] = 1LL;

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (str[j - 1] == sub[i - 1])
                    dp[i][j] = dp[i][j - 1] + dp[i - 1][j - 1];
                else
                    dp[i][j] = dp[i][j - 1];
            }
        }

        return dp[m][n];
    }
};