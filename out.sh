Command inputs: 
./testcase.txt,./output.txt
extractGridPositions
#################
#P S#* #*U#U  *S#
#   # 0#0 **0*  #
#U *S**#**S#* S*#
#  U0 ***S#* S#*#
#*#** ***# *  #*#
#U#S* #**#UU#  *#
#################
popped-item {
  currentPenguPosition: [ 1, 1 ],
  prevPenguPosition: [ 1, 1 ],
  fishesCaughtWhileTraversing: [],
  path: [],
  status: 'INITIAL'
}
popped-item {
  status: 'ON_SNOW',
  prevPenguPosition: [ 1, 1 ],
  fishesCaughtWhileTraversing: [ 'R3_C3' ],
  currentPenguPosition: [ 4, 4 ],
  path: [ 3 ]
}
popped-item {
  status: 'ON_SNOW',
  prevPenguPosition: [ 4, 4 ],
  fishesCaughtWhileTraversing: [ 'R3_C3' ],
  currentPenguPosition: [ 5, 3 ],
  path: [ 3, 1 ]
}
popped-item {
  status: 'STUCK_BY_WALL',
  prevPenguPosition: [ 5, 3 ],
  fishesCaughtWhileTraversing: [ 'R3_C3', 'R5_C4', 'R5_C6', 'R5_C7', 'R5_C8' ],
  currentPenguPosition: [ 5, 8 ],
  path: [ 3, 1, 6 ]
}
popped-item {
  status: 'STUCK_BY_WALL',
  prevPenguPosition: [ 5, 8 ],
  fishesCaughtWhileTraversing: [
    'R3_C3', 'R3_C6',
    'R4_C7', 'R5_C4',
    'R5_C6', 'R5_C7',
    'R5_C8'
  ],
  currentPenguPosition: [ 2, 5 ],
  path: [ 3, 1, 6, 7 ]
}
popped-item {
  status: 'STUCK_BY_WALL',
  prevPenguPosition: [ 2, 5 ],
  fishesCaughtWhileTraversing: [
    'R3_C3', 'R3_C5',
    'R3_C6', 'R4_C7',
    'R5_C4', 'R5_C6',
    'R5_C7', 'R5_C8'
  ],
  currentPenguPosition: [ 6, 5 ],
  path: [ 3, 1, 6, 7, 2 ]
}
popped-item {
  status: 'STUCK_BY_WALL',
  prevPenguPosition: [ 6, 5 ],
  fishesCaughtWhileTraversing: [
    'R1_C5', 'R3_C3',
    'R3_C5', 'R3_C6',
    'R4_C7', 'R5_C4',
    'R5_C6', 'R5_C7',
    'R5_C8'
  ],
  currentPenguPosition: [ 1, 5 ],
  path: [ 3, 1, 6, 7, 2, 8 ]
}
popped-item {
  status: 'STUCK_BY_WALL',
  prevPenguPosition: [ 6, 5 ],
  fishesCaughtWhileTraversing: [
    'R3_C3', 'R3_C5',
    'R3_C6', 'R3_C8',
    'R4_C7', 'R5_C4',
    'R5_C6', 'R5_C7',
    'R5_C8'
  ],
  currentPenguPosition: [ 2, 9 ],
  path: [ 3, 1, 6, 7, 2, 9 ]
}
popped-item {
  status: 'ON_SNOW',
  prevPenguPosition: [ 2, 9 ],
  fishesCaughtWhileTraversing: [
    'R2_C10', 'R2_C11',
    'R3_C3',  'R3_C5',
    'R3_C6',  'R3_C8',
    'R4_C7',  'R5_C4',
    'R5_C6',  'R5_C7',
    'R5_C8'
  ],
  currentPenguPosition: [ 2, 12 ],
  path: [
    3, 1, 6, 7,
    2, 9, 6
  ]
}
popped-item {
  status: 'ON_SNOW',
  prevPenguPosition: [ 2, 12 ],
  fishesCaughtWhileTraversing: [
    'R2_C10', 'R2_C11',
    'R3_C3',  'R3_C5',
    'R3_C6',  'R3_C8',
    'R4_C7',  'R5_C4',
    'R5_C6',  'R5_C7',
    'R5_C8'
  ],
  currentPenguPosition: [ 3, 12 ],
  path: [
    3, 1, 6, 7,
    2, 9, 6, 2
  ]
}
popped-item {
  status: 'STUCK_BY_WALL',
  prevPenguPosition: [ 3, 12 ],
  fishesCaughtWhileTraversing: [
    'R1_C14', 'R2_C10',
    'R2_C11', 'R2_C13',
    'R3_C3',  'R3_C5',
    'R3_C6',  'R3_C8',
    'R4_C7',  'R5_C4',
    'R5_C6',  'R5_C7',
    'R5_C8'
  ],
  currentPenguPosition: [ 1, 14 ],
  path: [
    3, 1, 6, 7, 2,
    9, 6, 2, 9
  ]
}
popped-item {
  status: 'STUCK_BY_WALL',
  prevPenguPosition: [ 1, 14 ],
  fishesCaughtWhileTraversing: [
    'R1_C14', 'R2_C10',
    'R2_C11', 'R2_C13',
    'R3_C12', 'R3_C3',
    'R3_C5',  'R3_C6',
    'R3_C8',  'R4_C11',
    'R4_C7',  'R5_C4',
    'R5_C6',  'R5_C7',
    'R5_C8'
  ],
  currentPenguPosition: [ 5, 10 ],
  path: [
    3, 1, 6, 7, 2,
    9, 6, 2, 9, 1
  ]
}
popped-item {
  status: 'STUCK_BY_WALL',
  prevPenguPosition: [ 5, 10 ],
  fishesCaughtWhileTraversing: [
    'R1_C14', 'R2_C10', 'R2_C11',
    'R2_C13', 'R3_C12', 'R3_C3',
    'R3_C5',  'R3_C6',  'R3_C8',
    'R4_C11', 'R4_C7',  'R5_C11',
    'R5_C4',  'R5_C6',  'R5_C7',
    'R5_C8'
  ],
  currentPenguPosition: [ 5, 13 ],
  path: [
    3, 1, 6, 7, 2,
    9, 6, 2, 9, 1,
    6
  ]
}
popped-item {
  status: 'STUCK_BY_WALL',
  prevPenguPosition: [ 5, 13 ],
  fishesCaughtWhileTraversing: [
    'R1_C14', 'R2_C10', 'R2_C11',
    'R2_C13', 'R3_C12', 'R3_C3',
    'R3_C5',  'R3_C6',  'R3_C8',
    'R4_C11', 'R4_C7',  'R5_C11',
    'R5_C4',  'R5_C6',  'R5_C7',
    'R5_C8'
  ],
  currentPenguPosition: [ 6, 13 ],
  path: [
    3, 1, 6, 7, 2,
    9, 6, 2, 9, 1,
    6, 2
  ]
}
popped-item {
  status: 'STUCK_BY_WALL',
  prevPenguPosition: [ 6, 13 ],
  fishesCaughtWhileTraversing: [
    'R1_C14', 'R2_C10', 'R2_C11',
    'R2_C13', 'R3_C12', 'R3_C3',
    'R3_C5',  'R3_C6',  'R3_C8',
    'R4_C11', 'R4_C7',  'R5_C11',
    'R5_C4',  'R5_C6',  'R5_C7',
    'R5_C8',  'R6_C15'
  ],
  currentPenguPosition: [ 6, 15 ],
  path: [
    3, 1, 6, 7, 2, 9,
    6, 2, 9, 1, 6, 2,
    6
  ]
}
popped-item {
  status: 'KILLED',
  prevPenguPosition: [ 6, 15 ],
  fishesCaughtWhileTraversing: [
    'R1_C14', 'R2_C10', 'R2_C11',
    'R2_C13', 'R3_C12', 'R3_C15',
    'R3_C3',  'R3_C5',  'R3_C6',
    'R3_C8',  'R4_C11', 'R4_C15',
    'R4_C7',  'R5_C11', 'R5_C15',
    'R5_C4',  'R5_C6',  'R5_C7',
    'R5_C8',  'R6_C15'
  ],
  currentPenguPosition: [ 1, 15 ],
  path: [
    3, 1, 6, 7, 2, 9,
    6, 2, 9, 1, 6, 2,
    6, 8
  ]
}
bfs: 15.925ms
Path length:14
