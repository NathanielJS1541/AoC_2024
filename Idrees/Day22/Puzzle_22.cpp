// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <cstdint>     // for fixed-width integer types like int64_t
// #include <string>      // for std::string

// using namespace std;

// // We prune by taking modulo with 16777216 (2^24).
// static const int64_t MOD = 16777216LL;

// // Performs one evolution of the secret as described in the puzzle.
// int64_t evolveSecret(int64_t secret) {
//     // Step 1: Multiply by 64, then XOR, then modulo
//     int64_t result = secret * 64LL;
//     secret ^= result;
//     secret %= MOD;

//     // Step 2: Divide by 32 (floor), then XOR, then modulo
//     result = secret / 32LL;
//     secret ^= result;
//     secret %= MOD;

//     // Step 3: Multiply by 2048, then XOR, then modulo
//     result = secret * 2048LL;
//     secret ^= result;
//     secret %= MOD;

//     return secret;
// }

// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);

//     // Read input file "input22.txt"
//     ifstream infile("input22.txt");
//     if (!infile.is_open()) {
//         cerr << "Error: Could not open input22.txt\n";
//         return 1;
//     }

//     vector<int64_t> initialSecrets;
//     {
//         int64_t val;
//         // Read each line as a 64-bit integer
//         while (infile >> val) {
//             initialSecrets.push_back(val);
//         }
//     }
//     infile.close();

//     int64_t totalSum = 0;

//     // For each buyer's initial secret:
//     for (auto secret : initialSecrets) {
//         // Generate 2000 new secret numbers
//         for (int i = 0; i < 2000; i++) {
//             secret = evolveSecret(secret);
//         }
//         // After 2000 evolutions, add to total sum
//         totalSum += secret;
//     }

//     // Print the final sum
//     cout << totalSum << "\n";

//     return 0;
// }




#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <limits>

// Pruning modulo (2^24)
static const int64_t MOD = 16777216LL;
// Number of possible changes per step is 19 (from -9..9)
static const int CHANGE_RANGE = 19;
static const int PATTERNS = CHANGE_RANGE * CHANGE_RANGE * CHANGE_RANGE * CHANGE_RANGE; // 19^4 = 130321

using namespace std;

// Evolve secret once:
int64_t evolveSecret(int64_t s) {
    // Step 1
    int64_t r = s * 64LL;
    s ^= r;
    s %= MOD;
    // Step 2
    r = s / 32LL;
    s ^= r;
    s %= MOD;
    // Step 3
    r = s * 2048LL;
    s ^= r;
    s %= MOD;
    return s;
}

// Convert 4 consecutive changes (c1, c2, c3, c4) in [-9..9] to a single index in [0..19^4-1].
inline int patternIndex(int c1, int c2, int c3, int c4) {
    // Shift from [-9..9] to [0..18]
    c1 += 9;
    c2 += 9;
    c3 += 9;
    c4 += 9;
    // Combine as base-19 number
    return ((((c1 * CHANGE_RANGE + c2) * CHANGE_RANGE + c3) * CHANGE_RANGE) + c4);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1) Read input from "input22.txt"
    ifstream fin("input22.txt");
    if (!fin.is_open()) {
        cerr << "Error: Cannot open input22.txt\n";
        return 1;
    }
    vector<int64_t> secrets;
    {
        int64_t val;
        while (fin >> val) {
            secrets.push_back(val);
        }
    }
    fin.close();
    int N = static_cast<int>(secrets.size());
    if (N == 0) {
        cout << 0 << "\n";
        return 0;
    }

    // We'll store, for each buyer, a "first occurrence" array of size PATTERNS (19^4).
    // Also store the 2001 prices for each buyer.
    // firstOccurrence[b][pattern] -> first index of that pattern, or -1 if none
    vector< vector<int> > firstOccurrence(N, vector<int>(PATTERNS, -1));

    // We'll store the prices for each buyer, to retrieve the sale price:
    // prices[b][i] => the i-th price (i from 0..2000)
    vector< vector<int> > buyerPrices(N);

    // 2) Precompute for each buyer:
    //    - The 2001 prices
    //    - The 2000 changes
    //    - Fill firstOccurrence[] with the first index of each 4-change pattern
    for (int b = 0; b < N; b++) {
        // Build the 2001 prices
        vector<int> prices;
        prices.reserve(2001);
        int64_t s = secrets[b];
        prices.push_back(static_cast<int>(s % 10));
        for (int i = 0; i < 2000; i++) {
            s = evolveSecret(s);
            prices.push_back(static_cast<int>(s % 10));
        }
        buyerPrices[b] = prices;

        // Build the 2000 changes
        vector<int> changes;
        changes.reserve(2000);
        for (int i = 0; i < 2000; i++) {
            changes.push_back(prices[i + 1] - prices[i]);
        }

        // Fill firstOccurrence[b]
        // Slide a length-4 window over changes[0..1999], i.e., i in [0..1996]
        // The pattern is (changes[i], changes[i+1], changes[i+2], changes[i+3]).
        for (int i = 0; i + 3 < 2000; i++) {
            int idx = patternIndex(changes[i], changes[i + 1], changes[i + 2], changes[i + 3]);
            // Only set if not yet set
            if (firstOccurrence[b][idx] == -1) {
                firstOccurrence[b][idx] = i;
            }
        }
    }

    // 3) Now find the best pattern. We'll do:
    //    bestSum = max over all patterns ( sum of sale prices across all buyers )
    long long bestSum = 0;

    // For each pattern p in [0..19^4-1], accumulate the sum of sale prices across all N buyers
    // The sale price for buyer b is:
    //    if firstOcc != -1, then buyerPrices[b][ firstOcc + 4 ]
    //    else 0
    for (int p = 0; p < PATTERNS; p++) {
        long long currentSum = 0;
        // Sum for all buyers
        for (int b = 0; b < N; b++) {
            int i = firstOccurrence[b][p];
            if (i != -1) {
                // The price at index i+4
                // i+4 <= 2000 for sure, so we can safely do buyerPrices[b][i+4].
                currentSum += buyerPrices[b][i + 4];
            }
        }
        if (currentSum > bestSum) {
            bestSum = currentSum;
        }
    }

    // 4) Print the best possible sum
    cout << bestSum << "\n";
    return 0;
}
