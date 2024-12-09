let read_input filename =
  let file = open_in filename in
  let lines = In_channel.input_lines file in
  close_in file;
  lines

let find_guard input =
  let rec check_rows rows index =
    match rows with
    | h :: _ when String.contains h '^' -> (index, String.index h '^')
    | _ :: t -> check_rows t (index + 1)
    | [] -> (-1, -1)
  in

  check_rows input 0

let pos_in_bounds pos rows cols =
  fst pos >= 0 && fst pos < rows && snd pos >= 0 && snd pos < cols

let turn_right direction = (snd direction, -fst direction)
let take_step pos dir = (fst pos + fst dir, snd pos + snd dir)

let move_until_of_bounds input =
  let rows = List.length input in
  let cols = String.length (List.nth input 0) in
  let guard_pos = find_guard input in

  let rec move_or_turn pos dir visited =
    let new_pos = take_step pos dir in
    match new_pos with
    | n when not (pos_in_bounds n rows cols) -> pos :: visited
    | n when (List.nth input (fst n)).[snd n] == '#' ->
        move_or_turn pos (turn_right dir) visited
    | n -> move_or_turn n dir (pos :: visited)
  in

  move_or_turn guard_pos (-1, 0) []

let silver () =
  let input = read_input "../inputs/06.txt" in
  let visited = move_until_of_bounds input in
  let uniq = List.sort_uniq (fun a b -> compare a b) visited in
  print_int (List.length uniq)
;;

silver ()

