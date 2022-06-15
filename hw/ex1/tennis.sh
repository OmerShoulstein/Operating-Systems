#!/bin/bash
p1points=50
p2points=50
current_position=0
game_over=false
invalid_move_prompt="NOT A VALID MOVE !"
winner=0

show_board() {
  echo " Player 1: ${p1points}         Player 2: ${p2points} "
  echo " --------------------------------- "
  echo " |       |       #       |       | "
  echo " |       |       #       |       | "
  case $current_position in
  0)
    echo " |       |       O       |       | "
    ;;
  -1)
    echo " |       |   O   #       |       | "
    ;;
  -2)
    echo " |   O   |       #       |       | "
    ;;
  -3)
    echo "O|       |       #       |       | "
    ;;
  1)
    echo " |       |       #   O   |       | "
    ;;
  2)
    echo " |       |       #       |   O   | "
    ;;
  3)
    echo " |       |       #       |       |O"
    ;;
  esac
  echo " |       |       #       |       | "
  echo " |       |       #       |       | "
  echo " --------------------------------- "
  if [ "$p1move" != "" ]; then
    echo -e "       Player 1 played: ${p1move}\n       Player 2 played: ${p2move}\n\n"
  fi
}

pick_move() {
  while true; do
    echo "PLAYER $1 PICK A NUMBER: "
    read -s -r move
    if [[ $move =~ ^[0-9]+$ ]]; then
      if [ "$1" == 1 ] && [ "$move" -gt $p1points ]; then
        echo "$invalid_move_prompt"
      else
        if [ "$1" == 2 ] && [ "$move" -gt $p2points ]; then
          echo "$invalid_move_prompt"
        else
          break
        fi
      fi
    else
      echo "$invalid_move_prompt"
    fi
  done
  return "$move"
}

play_turn() {
  pick_move 1
  p1move=$?
  pick_move 2
  p2move=$?
  if [ "$p1move" -gt "$p2move" ]; then
    if [ $current_position -lt 0 ]; then
      current_position=1
    else
      current_position=$((current_position + 1))
    fi
  elif [ "$p2move" -gt "$p1move" ]; then
    if [ $current_position -lt 0 ]; then
      current_position=$((current_position - 1))
    else
      current_position=-1
    fi
  fi
  p1points=$((p1points - p1move))
  p2points=$((p2points - p2move))

}
show_board
while [ $game_over = false ]; do
  play_turn
  show_board
  if [ $current_position -eq 3 ]; then
    game_over=true
    winner=1
  elif [ $current_position -eq -3 ]; then
    game_over=true
    winner=-1
  elif [ $p1points -eq 0 ] && [ $p2points -gt 0 ]; then
    game_over=true
    winner=-1
  elif [ $p2points -eq 0 ] && [ $p1points -gt 0 ]; then
    game_over=true
    winner=1
  elif [ $p1points -eq 0 ] && [ $p2points -eq 0 ]; then
    game_over=true
    if [ $current_position -gt 0 ]; then
      winner=1
    elif [ $current_position -lt 0 ]; then
      winner=-1
    fi
  else
    winner=0
  fi
done
case $winner in
0)
  echo "IT'S A DRAW !"
  ;;
1)
  echo "PLAYER 1 WINS !"
  ;;
-1)
  echo "PLAYER 2 WINS !"
  ;;
esac
