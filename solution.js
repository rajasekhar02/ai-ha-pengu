// #region imports
const PriorityQueue = require("./PriorityQueue");
const fs = require("fs").promises;
// #endregion imports

// #region Common Functions
const disableLogInFunctions = ["noteThePosition"];
const DEBUG_MODE = false;
var getType = function (obj) {
  return {}.toString
    .call(obj)
    .match(/\s([a-zA-Z]+)/)[1]
    .toLowerCase();
};
const printLog = function () {
  if (disableLogInFunctions.includes(arguments[0]) || !DEBUG_MODE) {
    return;
  }
  console.log(
    Array.from(arguments)
      .map((x) => {
        if (getType(x) === "object") {
          return JSON.stringify(x);
        }
        return x;
      })
      .join("\n")
  );
};
// #endregion Common Functions

// #region variables
const inputsFromCLI = process.argv.slice(2);
printLog("Command inputs: ", inputsFromCLI);
const inputFileName = inputsFromCLI[0];
const outputFileName = inputsFromCLI[1];
let gridRowsSize = 0;
let gridColsSize = 0;
let grid;
let discardSymbols = [" "];
let bearPositions = [];
let sharkPositions = [];
let wallPositions = [];
let fishPositions = [];
let snowPositions = [];
const symbolToName = {
  "*": "fish",
  0: "snow",
  S: "shark",
  U: "bear",
  "#": "wall"
};
const symbolToPositionsArray = {
  fish: fishPositions,
  snow: snowPositions,
  shark: sharkPositions,
  bear: bearPositions,
  wall: wallPositions
};
let penguPosition = [0, 0];
const directions = [
  undefined, // no move
  [1, -1], // bottom-left
  [1, 0], // bottom
  [1, 1], // bottom-right
  [0, -1], // left
  undefined, // no move
  [0, 1], // right
  [-1, -1], // top-left
  [-1, 0], // top
  [-1, 1] // top-right
];
const directionNames = [
  "no move",
  "bottom-left",
  "bottom",
  "bottom-right",
  "left",
  "no move",
  "right",
  "top-left",
  "top",
  "top-right"
];
let FISHES_TO_REACH_GOAL = 20
// #endregion variables

// #region helper functions

/**
 * Stores the given position <row, column> in the corresponding the symbol array
 *
 * @param {string} symbol it can be any symbol from the set of symbols ['*','U','S','#']
 * @param {number} rowPosition it is the row position in the grid
 * @param {number} colPosition it is the column position in the grid
 */
const noteThePosition = function (symbol, rowPosition, colPosition) {
  printLog(
    `noteThePosition`,
    `Symbol:${symbol}`,
    `rowPosition: ${rowPosition}`,
    `colPosition: ${colPosition}`
  );
  if (symbol === "P") {
    penguPosition = [rowPosition, colPosition];
    return;
  }
  symbolToPositionsArray[symbolToName[symbol]].push([rowPosition, colPosition]);
};

/**
 * Processes the array of strings.
 * Uses noteThePosition method to store the position in the corresponding symbol array
 *
 * @param {Array<string>} gridStrings it contains the array of strings each of length equal to variable gridColsSize and its length is equal to the variable gridRowsSize
 */
const extractGridPositions = function (gridStrings) {
  printLog("extractGridPositions", gridStrings.join("\n"));
  gridStrings.forEach((eachLine, rowPosition) => {
    const gridCells = eachLine.split("");
    gridCells.forEach((symbol, colPosition) => {
      if (discardSymbols.includes(symbol)) {
        return;
      }
      noteThePosition(symbol, rowPosition, colPosition);
    });
  });
};

/**
 * Checks whether the position is present in any of the symbols array
 *
 * @param {Array<number>} position it is an array of length 2 containing [rowPosition, columnPosition]
 * @param {string} item it can be any symbol from the set of symbols ['*','U','S','#']
 * @returns {boolean} if position exists in the symbolToPositionsArray return true
 *                    else return false
 */

const doesPositionHasGivenItem = function (position, item) {
  const truthValue = symbolToName[grid[position[0]][position[1]]] === item;
  return truthValue;
};

/**
 * Checks whether the current position of the pengu is occupied by the bear or shark
 *
 * @param {Array<number>} currentPosition it is an array of length 2 containing [rowPosition, columnPosition]
 * @returns if current position has a bear or shark it returns true
 *          else it return false
 */

const isPenguKilled = function (currentPosition) {
  return (
    doesPositionHasGivenItem(currentPosition, "bear") ||
    doesPositionHasGivenItem(currentPosition, "shark")
  );
};

/**
 * Creates an new position from the given current position and the direction
 *
 * @param {Array<number>} currentPosition it is an array of length 2 containing [rowPosition, columnPosition]
 * @param {number} direction it is a number represents the index of the direction array
 * @returns {Array<number>} newPosition is an array of length 2 containing [rowPosition, columnPosition]
 */
const getNewPosition = function (currentPosition, direction) {
  return [
    currentPosition[0] + directions[direction][0],
    currentPosition[1] + directions[direction][1]
  ];
};

/**
 * Checks whether the given position is invalid or not
 *
 * @param {Array<number>} position it is an array of length 2 containing [rowPosition, columnPosition]
 * @returns if position is out of the grid or has a wall then it returns true
 *          else it return false
 */
const checkAMoveIsInvalid = function (position) {
  const invalidMoves = [
    (position) => position[0] > gridRowsSize - 1 || position[0] <= 0,
    (position) => position[1] > gridColsSize - 1 || position[1] <= 0,
    // (position) => doesPositionHasGivenItem(position, "bear"),
    // (position) => doesPositionHasGivenItem(position, "shark"),
    (position) => doesPositionHasGivenItem(position, "wall")
  ];
  return invalidMoves.some((eachInvalidMoveFunc) => {
    return eachInvalidMoveFunc(position);
  });
};

/**
 * Generates all new positions by using the direction array and filters the valid positions
 *
 * @param {Array<number>} currentPosition it is an array of length 2 containing [rowPosition, columnPosition]
 * @param {number} direction it is a number represents the index of the direction array
 * @returns {Array<Array<number>>} array of valid positions generated based on the direction array
 */
const getValidPositions = function (currentPosition, direction) {
  const validPositions = [];
  for (
    directionIndex = 0;
    directionIndex < directions.length;
    directionIndex++
  ) {
    const eachDirection = directions[directionIndex];
    if (eachDirection === undefined || direction === directionIndex) continue;
    const newPosition = getNewPosition(currentPosition, directionIndex);
    if (checkAMoveIsInvalid(newPosition)) continue;
    validPositions.push({ position: newPosition, direction: directionIndex });
  }
  return validPositions;
};

/**
 * Checks whether it is possible to move futher in same direction based on the given currentPenguPosition and direction
 *
 * @param {Array<number>} currentPenguPosition it is an array of length 2 containing [rowPosition, columnPosition]
 * @param {number} direction it is a number represents the index of the direction array
 * @returns {boolean} if newMove from the currentPenguPosition and direction is not having a wall then it returns false
 *                    else true
 */
const isItPossibleToMoveFurtherInSameDirection = function (
  currentPenguPosition,
  direction
) {
  if (
    [" ", "*"].includes(grid[currentPenguPosition[0]][currentPenguPosition[1]])
  ) {
    const newMove = getNewPosition(currentPenguPosition, direction);
    if (doesPositionHasGivenItem(newMove, "wall")) {
      return false;
    } else {
      return true;
    }
  }
  return false;
};

/**
 * Casts the given state items into a string
 *
 * @param {number} pathLength it is an number containing the length of the path
 * @param {Array<number>} fromPosition it is an array of length 2 containing [rowPosition, columnPosition]
 * @param {Array<number>} toPosition it is an array of length 2 containing [rowPosition, columnPosition]
 * @param {Array<number>} fishesCaught it is an array of positions containing [rowPosition, columnPosition] of the fishes caught
 * @returns {string} it of the form R<row number>_C<column number>_R<row number>_C<column number>_F<fishesCaught>
 *
 */

const castPositionToString = function (position) {
  return `R${position[0]}_C${position[1]}`;
};


const castStateToString = function (fromPosition, toPosition, fishesCaught) {
  return `R${fromPosition[0]}_C${fromPosition[1]}__R${toPosition[0]}_C${toPosition[1]}_F${fishesCaught}`;
};

/**
 *
 * Function takes the currentState as input and traverse in the same direction until the below conditions are met:
 * 1. when pengu is killed by the shark or the bear
 * 2. when pengu reaches the snow cell
 * 3. when pengu is struck by the wall
 *
 * @param {Object< string, Array<string>, Array<number>, Array<number> >}
 */
const simulateTraversingInTheSameDirection = function (currentState) {
  const { currentPenguPosition, fishesCaughtWhileTraversing, path } =
    currentState;
  const currentMovingDirectionIndex = path[path.length - 1];
  const conditionToStopSimulation = [
    (position) => isPenguKilled(position),
    (position) => doesPositionHasGivenItem(position, "snow"),
    (position) =>
      doesPositionHasGivenItem(
        getNewPosition(position, currentMovingDirectionIndex),
        "wall"
      )
  ];

  let tempPosition = currentPenguPosition;
  let tempFishesCaughtWhileTraversing = [...fishesCaughtWhileTraversing];

  let conditionThatStoppedSimulation = conditionToStopSimulation
    .map((x) => x(tempPosition))
    .indexOf(true);
  while (conditionThatStoppedSimulation === -1) {
    if (
      doesPositionHasGivenItem(tempPosition, "fish") &&
      !tempFishesCaughtWhileTraversing.includes(
        castPositionToString(tempPosition)
      )
    ) {
      tempFishesCaughtWhileTraversing.push(castPositionToString(tempPosition));
    }
    tempPosition = getNewPosition(tempPosition, currentMovingDirectionIndex);
    conditionThatStoppedSimulation = conditionToStopSimulation
      .map((x) => x(tempPosition))
      .indexOf(true);
  }
  if (
    doesPositionHasGivenItem(tempPosition, "fish") &&
    !tempFishesCaughtWhileTraversing.includes(
      castPositionToString(tempPosition)
    )
  ) {
    tempFishesCaughtWhileTraversing.push(castPositionToString(tempPosition));
  }
  const conditionToStopSimulationMapStatus = [
    "KILLED",
    "ON_SNOW",
    "STUCK_BY_WALL"
  ];
  return {
    status: conditionToStopSimulationMapStatus[conditionThatStoppedSimulation],
    prevPenguPosition: currentState.prevPenguPosition,
    fishesCaughtWhileTraversing: tempFishesCaughtWhileTraversing,
    currentPenguPosition: tempPosition,
    path
  };
};

/**
 * Contains the implementation of the heuristic function
 * following heuristics are considered:
 *  1. Pengu in the state1 killed without reaching the goal of catching 20 fishes then state2 is considered as valid state for expansion.
 *  2. Pengu in the state2 killed without reaching the goal of catching 20 fishes then state1 is considered as valid state for expansion.
 * if function is not returned from step1 and step2 then Pengu should be alive, so we check the below conditions:
 *  1. Check whether is a difference between number of fishes eaten in state1 and state2 then return true if state1 has more fishes than state2 else return false
 *  2. if both state1 and state2 has same number of fishes then if state1 status is ON_SNOW then return true
 *  3. if both state1 and state2 has same number of fishes, state1 status is not equal ON_SNOW then if state2 status is ON_SNOW then return true
 *  4. if there state1 and state2 are similar then consider the lengths of the path for state1 and state2, return true if state1 has shorter path else false which mean state2 has shorter path
 * @param {Object< string, Array<string>, Array<number>, Array<number> >} state1 containing the status of the game
 * @param {Object< string, Array<string>, Array<number>, Array<number> >} state2 containing the status of the game
 * @returns boolean based on the above heuristics
 */

const heuristicFunction = function (state1, state2) {
  if (isPenguKilled(state1.currentPenguPosition) && state1.fishesCaughtWhileTraversing.length < 20) {
    return false;
  }
  if (isPenguKilled(state2.currentPenguPosition) && state2.fishesCaughtWhileTraversing.length < 20) {
    return true;
  }

  if (state1.fishesCaughtWhileTraversing.length - state2.fishesCaughtWhileTraversing.length !== 0) {
    return state1.fishesCaughtWhileTraversing.length > state2.fishesCaughtWhileTraversing.length;
  }
  if (state1.status === "ON_SNOW") {
    return true;
  }
  if (state2.status === "ON_SNOW") {
    return false
  }

  return state1.path.length < state2.path.length
}
/**
 *
 * Contains the implementation of the best first search over the game
 *
 * @param {Object< string, Array<string>, Array<number>, Array<number> >} initialState containing the status of the game,
 * fishes caught while traversing, direction visited as path, current pengu location
 */
const findRouteUsingBestFSFrom = function (initialState) {
  const priorityQueue = new PriorityQueue(heuristicFunction);
  priorityQueue.push(initialState);
  let currentState;
  const visitedStates = new Set();
  while (!priorityQueue.isEmpty()) {
    currentState = priorityQueue.pop();
    printLog(`popped-item:${currentState}`)
    if (currentState.fishesCaughtWhileTraversing.length >= FISHES_TO_REACH_GOAL) {
      currentState.status = isPenguKilled(currentState.currentPenguPosition)
        ? "KILLED" : "VICTORY";
      return currentState;
    }
    if (isPenguKilled(currentState.currentPenguPosition)) {
      continue;
    }
    getValidPositions(currentState.currentPenguPosition).forEach(
      (eachValidMove) => {
        let copyOfCurrentState = JSON.parse(JSON.stringify(currentState));
        copyOfCurrentState.currentPenguPosition = eachValidMove.position;
        copyOfCurrentState.prevPenguPosition = currentState.currentPenguPosition;
        copyOfCurrentState.path.push(eachValidMove.direction);
        if (
          [" ", "*"].includes(
            grid[copyOfCurrentState.currentPenguPosition[0]][
            copyOfCurrentState.currentPenguPosition[1]
            ]
          )
        ) {
          copyOfCurrentState = simulateTraversingInTheSameDirection(copyOfCurrentState);
        }
        const visitedStateString = castStateToString(
          currentState.currentPenguPosition,
          copyOfCurrentState.currentPenguPosition,
          copyOfCurrentState.fishesCaughtWhileTraversing.sort().join(""),

        );
        if (!visitedStates.has(visitedStateString)) {
          visitedStates.add(visitedStateString);
          priorityQueue.push(copyOfCurrentState);
        }
      }
    );
  }

  return {
    status: isPenguKilled(currentState.currentPenguPosition)
      ? "KILLED"
      : "FAILED",
    currentPenguPosition: currentState.currentPenguPosition,
    fishesCaughtWhileTraversing: currentState.fishesCaughtWhileTraversing,
    path: currentState.path
  };
};

/**
 * Generates an output file by using the outputObj
 *
 * @param {Object< boolean, Array<string>,Array<number>, Array<number> >} outputObj it is an object containing the status of the game,
 * fishes caught while traversing, direction visited as path, current pengu location
 */
const generateOutputFile = async function (outputObj) {
  try {
    const { status, path, fishesCaughtWhileTraversing, currentPenguPosition } =
      outputObj;
    printLog(`Path length:${path.length}`)
    const updatedGrid = grid
      .map((row, indexRow) => {
        return row
          .map((col, indexCol) => {
            const foundFishPosition = fishesCaughtWhileTraversing.includes(
              castPositionToString([indexRow, indexCol])
            );
            if (
              currentPenguPosition[0] === indexRow &&
              currentPenguPosition[1] === indexCol
            ) {
              return status === "KILLED" ? "X" : "P";
            }
            if (foundFishPosition) {
              return " ";
            }
            return col;
          })
          .join("");
      })
      .join("\n");
    await fs.writeFile(
      outputFileName,
      [path.join(""), fishesCaughtWhileTraversing.length, updatedGrid].join(
        "\n"
      )
    );
  } catch (err) {
    console.log("something went wrong", err);
  }
};
// #endregion function

// #region main function
/**
 * Main function that drives the solution
 */
(async function () {
  try {
    const data = await fs.readFile(inputFileName, "utf8");
    const textFileLines = data.split("\n").map((eachLine) => eachLine.trim());
    // assign the grid dimensions
    [gridRowsSize, gridColsSize] = textFileLines[0]
      .split(" ")
      .map((gridDimension) => +gridDimension);
    const gridStrings = textFileLines.slice(1);
    grid = gridStrings.map((eachStr) => eachStr.split(""));

    // create the grid
    extractGridPositions(gridStrings);
    grid[penguPosition[0]][penguPosition[1]] = " ";
    console.time("Best-First-Search");
    let result = findRouteUsingBestFSFrom({
      currentPenguPosition: penguPosition,
      prevPenguPosition: penguPosition,
      fishesCaughtWhileTraversing: [],
      path: [],
      status: "INITIAL"
    });
    console.timeEnd("Best-First-Search");
    // findRouteFrom(penguPosition, [], []);
    await generateOutputFile(result);

  } catch (err) {
    console.log("something went wrong", err);
  }
})();
// #endregion main function
