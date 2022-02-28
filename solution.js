// #region imports
const fs = require("fs").promises;
// #endregion imports

// #region Common Functions
const disableLogInFunctions = ["noteThePosition"];
const DEBUG_MODE = true;
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
const getRandomIndex = function (min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1) + min); //The maximum is inclusive and the minimum is inclusive
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
  const truthValue = symbolToPositionsArray[item].some((eachItemPosition) => {
    return (
      eachItemPosition[0] === position[0] && eachItemPosition[1] === position[1]
    );
  });
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

const getValidPositions = function (currentPosition, direction) {
  const validMoves = [];
  for (
    directionIndex = 0;
    directionIndex < directions.length;
    directionIndex++
  ) {
    const eachDirection = directions[directionIndex];
    if (eachDirection === undefined || direction === directionIndex) continue;
    const newPosition = getNewPosition(currentPosition, directionIndex);
    if (checkAMoveIsInvalid(newPosition)) continue;
    validMoves.push({ position: newPosition, direction: directionIndex });
  }
  return validMoves;
};

const isItPossibleToMoveFurtherInSameDirection = function (
  currentPenguLocation,
  direction
) {
  if (
    [" ", "*"].includes(grid[currentPenguLocation[0]][currentPenguLocation[1]])
  ) {
    const newMove = getNewPosition(currentPenguLocation, direction);
    if (doesPositionHasGivenItem(newMove, "wall")) {
      return false;
    } else {
      return true;
    }
  }
  return false;
};

const castPositionToString = function (position) {
  return `R${position[0]}_C${position[1]}`;
};

const findRouteFrom = function (
  currentPenguLocation,
  path,
  fishesCaughtWhileTraversing
) {
  const currentMovingDirectionIndex = path[path.length - 1];
  const conditionsToCallGetValidMoves = [
    () => path.length === 0,
    (position) => doesPositionHasGivenItem(position, "snow"),
    (position) =>
      !isItPossibleToMoveFurtherInSameDirection(
        position,
        currentMovingDirectionIndex
      )
  ];
  const returnObjectFunc = (status, penguPosition, finalPath) => {
    return {
      status,
      fishesCaughtWhileTraversing,
      path: finalPath,
      currentPenguLocation: penguPosition
    };
  };
  const canSelectNewMoves = conditionsToCallGetValidMoves.some((x) =>
    x(currentPenguLocation)
  );
  // Bear or Shark killed the pengu
  if (isPenguKilled(currentPenguLocation)) {
    return returnObjectFunc("KILLED", currentPenguLocation, path);
  }
  // eat the fish if the position has any
  if (
    doesPositionHasGivenItem(currentPenguLocation, "fish") &&
    !fishesCaughtWhileTraversing.includes(
      castPositionToString(currentPenguLocation)
    )
  ) {
    fishesCaughtWhileTraversing.push(
      castPositionToString(currentPenguLocation)
    );
  }
  // if all the fishes in the grid are eaten then victory
  if (
    fishesCaughtWhileTraversing.length === fishPositions.length &&
    canSelectNewMoves
  ) {
    return returnObjectFunc("VICTORY", currentPenguLocation, path);
  }
  // stop traversing over the grid when pengu completed 6 moves and has a chance to select the next move
  if (path.length >= 6 && canSelectNewMoves) {
    return returnObjectFunc("GAME_OVER", currentPenguLocation, path);
  }
  // Triggers when pengu reaches the snow cell or stopped by hitting the wall or at the start of the traversal
  if (canSelectNewMoves) {
    const validMoves = getValidPositions(currentPenguLocation);
    let randomDirection = getRandomIndex(0, validMoves.length - 1);
    let eachMove = validMoves[randomDirection].position;
    let eachDirection = validMoves[randomDirection].direction;
    if (isPenguKilled(eachMove)) {
      return returnObjectFunc("KILLED", eachMove, [...path, eachDirection]);
    }
    let routeStatus = findRouteFrom(
      eachMove,
      [...path, eachDirection],
      fishesCaughtWhileTraversing
    );
    return routeStatus;
  }
  const futherMove = getNewPosition(
    currentPenguLocation,
    currentMovingDirectionIndex
  );
  if (!checkAMoveIsInvalid(futherMove)) {
    const routeStatus = findRouteFrom(
      futherMove,
      path,
      fishesCaughtWhileTraversing
    );
    return routeStatus;
  }
  return returnObjectFunc("END", currentPenguLocation, path);
};

const generateOutputFile = async function (outputObj) {
  try {
    const { status, path, fishesCaughtWhileTraversing, currentPenguLocation } =
      outputObj;
    const updatedGrid = grid
      .map((row, indexRow) => {
        return row
          .map((col, indexCol) => {
            const foundFishPosition = fishesCaughtWhileTraversing.includes(
              castPositionToString([indexRow, indexCol])
            );
            if (
              currentPenguLocation[0] === indexRow &&
              currentPenguLocation[1] === indexCol
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
    let result = findRouteFrom(penguPosition, [], []);
    await generateOutputFile(result);
  } catch (err) {
    console.log("something went wrong", err);
  }
})();

// #endregion main function
