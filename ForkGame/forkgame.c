#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*
  fork-game (RPS decision tree)
  - each process represents one possible path in the decision tree
  - on each step (round), we fork into possible outcomes
  - parent waits for the children and exits
  - children keep going deeper into the tree
  - we prune tiny probability branches with prob_min (same idea as sailor)
  - instead of position (x,y), we track scores in Rock Paper Scissors.
*/

static int rounds = 5;                 // keep small so process count < 1000
static double probR = 0.333333;        // opponent plays Rock with this prob
static double probP = 0.333333;        // opponent plays Paper
static double probS = 0.333333;        // opponent plays Scissors
static double prob_min = 1e-6;         // prune low probability branches (sailor idea)

static char strategy[256] = "RPSRP";   // my moves, repeats if rounds > length

//game state
static int round_i = 0;
static int myScore = 0;
static int oppScore = 0;
static double prob = 1.0;              // probability of reaching this state

static char my_move_at(int i) {
  // If strategy is shorter than rounds, just cycle through it.
  int n = (int)strlen(strategy);
  if (n == 0) return 'R';              // default if someone passes empty strategy
  return strategy[i % n];
}

static int outcome(char me, char opp) {
  /*
    returns:
      +1 if I win
       0 if tie
      -1 if I lose
    ode to vgd last sem
  */
  if (me == opp) return 0;
  if (me == 'R' && opp == 'S') return +1;
  if (me == 'P' && opp == 'R') return +1;
  if (me == 'S' && opp == 'P') return +1;
  return -1;
}

static void apply_opp_move(char oppMove, double oppProb) {
  /*
    This function "takes one step" in the game for this path.

    Similar to sailor game updating (x,y) and prob when choosing a direction:
    - update probability
    - advance step counter
    - update state variables (scores)
  */
  char me = my_move_at(round_i);
  int o = outcome(me, oppMove);

  prob *= oppProb;     // like sailor multiplying by move probability
  round_i += 1;

  if (o > 0) myScore += 1;
  else if (o < 0) oppScore += 1;
  // ties do not change score
}

static void init(int argc, char *argv[]) {
  /*
    rescale probabilities to sum to 1 (same idea as sailor init does).
  */
  for (int i = 1; i < argc; i++) {
    const char *a;

    a = "--rounds=";
    if (strncmp(argv[i], a, strlen(a)) == 0) {
      rounds = atoi(argv[i] + strlen(a));
      continue;
    }

    a = "--strategy=";
    if (strncmp(argv[i], a, strlen(a)) == 0) {
      strncpy(strategy, argv[i] + strlen(a), sizeof(strategy) - 1);
      strategy[sizeof(strategy) - 1] = '\0';
      continue;
    }

    a = "--prob-R=";
    if (strncmp(argv[i], a, strlen(a)) == 0) {
      probR = atof(argv[i] + strlen(a));
      continue;
    }

    a = "--prob-P=";
    if (strncmp(argv[i], a, strlen(a)) == 0) {
      probP = atof(argv[i] + strlen(a));
      continue;
    }

    a = "--prob-S=";
    if (strncmp(argv[i], a, strlen(a)) == 0) {
      probS = atof(argv[i] + strlen(a));
      continue;
    }

    a = "--prob-min=";
    if (strncmp(argv[i], a, strlen(a)) == 0) {
      prob_min = atof(argv[i] + strlen(a));
      continue;
    }

    fprintf(stderr, "Unknown arg: %s\n", argv[i]);
    exit(1);
  }

  // Rescale so probR + probP + probS = 1.0 (same style idea as sailor)
  double sum = probR + probP + probS;
  if (sum <= 0) {
    fprintf(stderr, "Probabilities must be positive.\n");
    exit(1);
  }
  probR /= sum;
  probP /= sum;
  probS /= sum;
}

int main(int argc, char *argv[]) {
  // From class
  setbuf(stdout, NULL);

  init(argc, argv);

  //checking or redirecting output
  printf("result,rounds,myScore,oppScore,prob,pid\n");

  /*
    Main loop:
    Each iteration is one "round".
  */
  while (round_i < rounds) {
    pid_t kids[3] = {-1, -1, -1};     // store child pids (for parent wait)
    double p[3] = {probR, probP, probS};
    char mv[3] = {'R', 'P', 'S'};

    int forked_as_child = 0;          // if I become a child, I keep exploring

    // Try the 3 possible opponent moves
    for (int k = 0; k < 3; k++) {
      // Sailor style prune: do not spawn branches with tiny probability
      if (prob * p[k] <= prob_min) continue;

      pid_t pid = fork();             // from class: fork makes a new process
      if (pid < 0) {
        perror("fork");
        exit(1);
      }

      if (pid == 0) {
        /*
          Child process:
          - apply this branch outcome (opp move)
          - break so this child continues the while loop on the next round
          This is the same "child keeps going" behavior as sailor.
        */
        apply_opp_move(mv[k], p[k]);
        forked_as_child = 1;
        break;
      } else {
        // Parent process:
        // remember pid so we can waitpid later (like sailor game waits)
        kids[k] = pid;
      }
    }

    if (!forked_as_child) {
      /*
        Parent path:
        If we get here, this process is the parent for this round.
        It created children for the branches. It waits for them, then exits.

        This is the important pattern:
        - parent does NOT keep exploring deeper
        - only children explore deeper
      */
      for (int k = 0; k < 3; k++) {
        if (kids[k] > 0) waitpid(kids[k], NULL, 0);
      }
      return 0;
    }

    // If forked_as_child == 1, we keep looping and create the next round branches.
  }

  // If we finished rounds, decide final result for this path
  const char *result = "TIE";
  if (myScore > oppScore) result = "WIN";
  else if (oppScore > myScore) result = "LOSE";

  // Print final leaf node result
  printf("%s,%d,%d,%d,%lg,%d\n", result, rounds, myScore, oppScore, prob, getpid());
  return 0;
}