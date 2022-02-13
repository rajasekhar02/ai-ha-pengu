// #region imports
const fs = require("fs").promises;
// #endregion imports

// #region Common Functions
const disableLogInFunctions = ["noteThePosition"];
const DEBUG_MODE = true;
const printLog = function () {
  if (disableLogInFunctions.includes(arguments[0]) || !DEBUG_MODE) {
    return;
  }
  console.log(Array.from(arguments).join("\n"));
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
let totalValidCells = 0;
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
const antiDirections = {
  "no move": 0,
  "bottom-left": 9, //"top-right",
  bottom: 8, //"top",
  "bottom-right": 7, //"top-left",
  left: 6, //"right",
  "no move": 5, // "no move",
  right: 4, //"left",
  "top-left": 3, //"bottom-right",
  top: 2, //"bottom",
  "top-right": 1 // "bottom-left"
};
// #endregion variables

// #region helper functions
const getNewCellState = function () {
  const cellState = directionNames.reduce((acc, curr) => {
    acc[curr] = "SAFE";
    return acc;
  }, {});
  cellState.visited_freq = 0;
  return cellState;
};
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
const countTotalValidCells = function () {
  for (let i = 1; i < gridRowsSize; i++) {
    for (let j = 1; j < gridColsSize; j++) {
      if (
        !checkAMoveIsInvalid([i, j]) &&
        getValidPositions([i, j]).length > 0
      ) {
        totalValidCells++;
      }
    }
  }
};

const doesPositionHasGivenItem = function (position, item) {
  const truthValue = symbolToPositionsArray[item].some((eachItemPosition) => {
    return (
      eachItemPosition[0] === position[0] && eachItemPosition[1] === position[1]
    );
  });
  return truthValue;
};

const isFishCaptured = function (capturedFishPositions, position) {
  return capturedFishPositions.some((eachFishPosition) => {
    return (
      eachFishPosition[0] === position[0] && eachFishPosition[1] === position[1]
    );
  });
};

const checkAMoveIsInvalid = function (position) {
  const invalidMoves = [
    (position) => position[0] > gridRowsSize - 1 || position[0] <= 0,
    (position) => position[1] > gridColsSize - 1 || position[1] <= 0,
    (position) => doesPositionHasGivenItem(position, "bear"),
    (position) => doesPositionHasGivenItem(position, "shark"),
    (position) => doesPositionHasGivenItem(position, "wall")
  ];
  return invalidMoves.some((eachInvalidMoveFunc) => {
    // printLog(eachInvalidMoveFunc(position), position);
    return eachInvalidMoveFunc(position);
  });
};
const checkAllCellsVisitedAleastOnce = function (positionAndFrequencyKey) {
  return Object.keys(positionAndFrequencyKey).length === totalValidCells;
};
const doesAllFishesCapured = function (capturedFishes) {
  return capturedFishes.length === fishPositions.length;
};
const getNewMove = function (currentMove, direction) {
  // printLog(direction, directions[direction]);
  return [
    currentMove[0] + directions[direction][0],
    currentMove[1] + directions[direction][1]
  ];
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
    // printLog(
    //   checkAMoveIsInvalid(newPosition, capturedFishes),
    //   (path.length === 0 ? [0] : path).map((x) => directionNames[x]),
    //   `StartPosition:${startPosition}`,
    //   `newPosition:${newPosition}`,
    //   `freq:${positionAndFrequency[positionAndFrequencyKey]}`
    // );
    if (checkAMoveIsInvalid(newPosition)) continue;
    validMoves.push({ position: newPosition, direction: directionIndex });
  }
  return validMoves;
};

const sortValidPositionsByVisitingFreq = function (
  validMoves,
  visitedPositionsFreq
) {
  // Always take the unexplored moves first
  const positionAndFrequencyKey = (position) =>
    `R${position[0]}_C${position[1]}`;
  // .filter((x) => {
  //   return (
  //     !visitedPositionsFreq[positionAndFrequencyKey(x.position)] ||
  //     visitedPositionsFreq[positionAndFrequencyKey(x.position)].visited_freq >
  //       1
  //   );
  // })
  return validMoves.sort((a, b) => {
    const positionA = a.position;
    const positionB = b.position;
    if (!visitedPositionsFreq[positionAndFrequencyKey(positionA)]) {
      return -1;
    }
    if (!visitedPositionsFreq[positionAndFrequencyKey(positionB)]) {
      return 1;
    }
    return (
      visitedPositionsFreq[positionAndFrequencyKey(positionA)].visited_freq -
      visitedPositionsFreq[positionAndFrequencyKey(positionB)].visited_freq
    );
  });
};

const moveAroundTheGrid = function (
  startPosition,
  path,
  capturedFishes,
  positionAndFrequency
) {
  const logInvalidMessage = (message) => {
    printLog(
      message,
      startPosition,
      (path.length === 0 ? [0] : path).map((dir) => directionNames[dir]),
      capturedFishes.length === 0
        ? "NO_FISHES_CAPTURED"
        : "Fishes Captured: " + capturedFishes.length
    );
  };
  const invalidMessage = [
    [checkAMoveIsInvalid(startPosition), "invalid move"],
    [doesAllFishesCapured(capturedFishes), "Captured all fishes"],
    [
      checkAllCellsVisitedAleastOnce(positionAndFrequency),
      "visited every cell in grid aleast once"
    ]
  ].find((condition) => {
    return condition[0];
  });
  printLog(invalidMessage);
  if (invalidMessage) {
    logInvalidMessage(invalidMessage[1]);
    return invalidMessage[1];
  }
  // counting the # of captured fishes
  if (
    doesPositionHasGivenItem(startPosition, "fish") &&
    !isFishCaptured(capturedFishes, startPosition)
  ) {
    printLog(
      "captured the fish",
      (path.length === 0 ? [0] : path).map((dir) => directionNames[dir]),
      startPosition
    );
    capturedFishes.push(startPosition);
  }

  const direction = path[path.length - 1];
  //snow field
  if (path.length === 0 || doesPositionHasGivenItem(startPosition, "snow")) {
    let eachMove = 0;
    const validPositions = sortValidPositionsByVisitingFreq(
      getValidPositions(
        startPosition,
        antiDirections[directionNames[direction]]
      ),
      positionAndFrequency
    );
    printLog(validPositions);
    for (
      eachPositionIndex = 0;
      eachPositionIndex < validPositions.length;
      eachPositionIndex++
    ) {
      const newPosition = validPositions[eachPositionIndex].position;
      const newDirection = validPositions[eachPositionIndex].direction;
      if (
        positionAndFrequency[`R${newPosition[0]}_C${newPosition[1]}`] ===
        undefined
      ) {
        positionAndFrequency[`R${newPosition[0]}_C${newPosition[1]}`] =
          getNewCellState();
      }
      printLog(
        `selected direction: ${
          directionNames[newDirection]
        } to ${startPosition} ${
          path.length === 0
            ? `init ${validPositions.map((x) => x.position)}`
            : "entered snow"
        }`
      );
      positionAndFrequency[`R${newPosition[0]}_C${newPosition[1]}`][
        directionNames[direction]
      ] = moveAroundTheGrid(
        newPosition,
        [...path, newDirection],
        [...capturedFishes],
        { ...positionAndFrequency }
      );
      // printLog(
      //   Object.values(positionAndFrequency || {}).map((eachValue) =>
      //     Object.values(eachValue)
      //   )
      // );
      positionAndFrequency[`R${newPosition[0]}_C${newPosition[1]}`]
        .visited_freq++;
    }
    printLog(
      "moves over",
      startPosition,
      (path.length === 0 ? [0] : path).map((x) => directionNames[x]),
      capturedFishes.length === 0
        ? "NO_FISHES_CAPTURED"
        : "Fishes Captured: " + capturedFishes.length
    );
    return "END";
  }

  if ([" ", "*"].includes(grid[startPosition[0]][startPosition[1]])) {
    // if i am stopping in next move, can i move in another direction
    const newMove = getNewMove(startPosition, direction);
    if (doesPositionHasGivenItem(newMove, "wall")) {
      let eachMove = 0;
      const validPositions = sortValidPositionsByVisitingFreq(
        getValidPositions(
          startPosition,
          antiDirections[directionNames[direction]]
        ),
        positionAndFrequency
      );
      // printLog(validPositions);
      for (
        eachPositionIndex = 0;
        eachPositionIndex < validPositions.length;
        eachPositionIndex++
      ) {
        const newPosition = validPositions[eachPositionIndex].position;
        const newDirection = validPositions[eachPositionIndex].direction;
        if (
          positionAndFrequency[`R${newPosition[0]}_C${newPosition[1]}`] ===
          undefined
        ) {
          positionAndFrequency[`R${newPosition[0]}_C${newPosition[1]}`] =
            getNewCellState();
        }
        printLog(
          `selected direction: ${directionNames[newDirection]} to ${startPosition}  blocked by wall`
        );
        positionAndFrequency[`R${newPosition[0]}_C${newPosition[1]}`][
          directionNames[direction]
        ] = moveAroundTheGrid(
          newPosition,
          [...path, newDirection],
          [...capturedFishes],
          positionAndFrequency
        );
        // printLog(
        //   Object.values(positionAndFrequency || {}).map((eachValue) =>
        //     Object.values(eachValue)
        //   )
        // );
        positionAndFrequency[`R${newPosition[0]}_C${newPosition[1]}`]
          .visited_freq++;
      }
      printLog(
        "moves over",
        startPosition,
        (path.length === 0 ? [0] : path).map((x) => directionNames[x]),
        capturedFishes.length === 0
          ? "NO_FISHES_CAPTURED"
          : "Fishes Captured: " + capturedFishes.length
      );
      return "END";
    } else {
      if (positionAndFrequency[`R${newMove[0]}_C${newMove[1]}`] === undefined) {
        positionAndFrequency[`R${newMove[0]}_C${newMove[1]}`] =
          getNewCellState();
      }
      printLog(`Moving Further ${directionNames[path[path.length - 1]]}...`);
      positionAndFrequency[`R${newMove[0]}_C${newMove[1]}`][direction] =
        moveAroundTheGrid(
          newMove,
          [...path], // no need to insert direction as it is continuing in the previous direction
          [...capturedFishes],
          positionAndFrequency
        );
      printLog(
        Object.values(positionAndFrequency || {}).map((eachValue) =>
          Object.values(eachValue)
        )
      );
      positionAndFrequency[`R${newMove[0]}_C${newMove[1]}`].visited_freq++;
      return "END";
    }
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
    printLog(
      "main",
      `gridRowsSize:${gridRowsSize}`,
      `gridColsSize:${gridColsSize}`
    );
    printLog(
      Object.keys(symbolToName)
        .map(
          (eachSymbol) =>
            `${symbolToName[eachSymbol]}:${symbolToPositionsArray[
              symbolToName[eachSymbol]
            ].map((eachPosition) => `[${eachPosition[0]},${eachPosition[1]}]`)}`
        )
        .join("\n")
    );
    countTotalValidCells();
    grid[penguPosition[0]][penguPosition[1]] = " ";
    moveAroundTheGrid(penguPosition, [], [], {
      [`R${penguPosition[0]}_C${penguPosition[1]}`]: getNewCellState()
    });

    // printLog(
    //   sortValidPositionsByVisitingFreq(getValidPositions([3, 2]), {
    //     [`R2_C3`]: { visited_freq: 3 },
    //     [`R2_C1`]: { visited_freq: 3 }
    //   }).map((x) => x.position)
    // );
    // printLog(totalValidCells);
  } catch (err) {
    console.log("something went wrong", err);
  }
})();

// #endregion main function
