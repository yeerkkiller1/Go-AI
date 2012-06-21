/*
 * dummy.c, computer go test player
 * Copyright (C) 1995-1996 William Shubert
 */


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "dummy.h"
#include "gmp.h"


/**********************************************************************
 * Forward declarations
 **********************************************************************/

static void  waitForNewGame(Gmp *ge);
static void  playGame(Gmp *ge, int size, int himFirst);


/**********************************************************************
 * Functions
 **********************************************************************/

int main(int argc, char *argv[])  {
  Gmp  *ge;

  srand(time(NULL));
  ge = gmp_create(0, 1);

  for (;;)  {
    /*
     * I really should look at the command line and set up the handicap,
     *   board size, etc. better than this.
     */
    gmp_startGame(ge, -1, -1, 5.5, -1, -1);
    waitForNewGame(ge);
    if (gmp_handicap(ge) != 0)  {
      fprintf(stderr, "I don't know how to play handicap games.\n");
      exit(1);
    }
    playGame(ge, gmp_size(ge), gmp_iAmWhite(ge));
  }
  exit(0);
}


static void  waitForNewGame(Gmp *ge)  {
  GmpResult  result;
  const char  *err;

  do  {
    result = gmp_check(ge, 1, NULL, NULL, &err);
  } while ((result == gmp_nothing) || (result == gmp_reset));
  if (result == gmp_err)  {
    fprintf(stderr, "dummy: Error \"%s\" occurred.\n", err);
    exit(1);
  } else if (result != gmp_newGame)  {
    fprintf(stderr, "Expecting a newGame, got %s\n",
	    gmp_resultString(result));
    exit(1);
  }
}


/*
 * This is just about the world's stupidest go player.  It picks random
 *   locations, checking for moves with at least one liberty.  If it finds
 *   one, it makes the move.  Otherwise it tries again.  Eventually it will
 *   give up and pass.
 * It is so stupid it doesn't even deal with captures or undos.
 */
static void  playGame(Gmp *ge, int size, int himFirst)  {
  int  *board, i, x, y;
  int  moveMade, passes = 0;
  GmpResult  message;
  const char  *error;

  board = malloc(size * size * sizeof(int));
  for (i = 0;  i < size * size;  ++i)
    board[i] = 0;
  for (;;)  {
    if (!himFirst)  {
      moveMade = 0;
      for (i = 0;  (i < 50) && !moveMade;  ++i)  {
	x = rand() % size;
	y = rand() % size;
	if ((board[x + y * size] == 0) &&
	    (((x > 0) && (board[(x - 1) + y * size] == 0)) ||
	     ((y > 0) && (board[x + (y - 1) * size] == 0)) ||
	     ((x < size-1) && (board[(x + 1) + y * size] == 0)) ||
	     ((y < size-1) && (board[x + (y + 1) * size] == 0))))  {
	  passes = 0;
	  gmp_sendMove(ge, x, y);
	  board[x + y * size] = 1;
	  moveMade = 1;
	}
      }
      if (!moveMade)  {
	gmp_sendPass(ge);
	if (++passes == 2)  {
	  /*
	   * Game over!
	   */
	  return;
	}
      }
    }
    himFirst = 0;
    do  {
      message = gmp_check(ge, 1, &x, &y, &error);
    } while (message == gmp_nothing);
    if (message == gmp_err)  {
      fprintf(stderr, "dummy: Got error \"%s\"\n", error);
      exit(1);
    }
    if (message == gmp_move)  {
      passes = 0;
      board[x + y * size] = 1;
    } else if (message == gmp_pass)  {
      if (++passes == 2)  {
	/*
	 * Game over!
	 */
	return;
      }
    } else if (message == gmp_reset)  {
      /* Game over! */
      return;
    } else if (message == gmp_undo)  {
      fprintf(stderr, "dummy: I am asked to undo %d moves.\n", x);
      if ((x & 1) == 0)  {
	/*
	 * Since an even number of undos were made, it is still the other
	 *   player's turn.  This flag makes me skip my turn.
	 */
	himFirst = 1;
      }
    } else  {
      fprintf(stderr, "Got a \"%s\" command during game play.  Exiting.\n",
	      gmp_resultString(message));
      exit(1);
    }
  }
}
