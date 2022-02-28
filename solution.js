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

const doesPositionHasGivenItem = function (position, item) {
  const truthValue = symbolToPositionsArray[item].some((eachItemPosition) => {
    return (
      eachItemPosition[0] === position[0] && eachItemPosition[1] === position[1]
    );
  });
  return truthValue;
};

const isPenguKilled = function (currentPosition) {
  return (
    doesPositionHasGivenItem(currentPosition, "bear") ||
    doesPositionHasGivenItem(currentPosition, "shark")
  );
};

const getNewMove = function (currentMove, direction) {
  return [
    currentMove[0] + directions[direction][0],
    currentMove[1] + directions[direction][1]
  ];
};

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
    const newPosition = getNewMove(currentPosition, directionIndex);
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
    const newMove = getNewMove(currentPenguLocation, direction);
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
  const futherMove = getNewMove(
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
