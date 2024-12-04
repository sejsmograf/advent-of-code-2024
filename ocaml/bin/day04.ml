let read_input filename =
  let file = open_in filename in
  let lines = In_channel.input_lines file in
  close_in file;
  lines

let find_string_in_direction row col str grid direction =
  let target_len = String.length str in
  let gridRows = List.length grid in
  let gridCols = String.length (List.nth grid 0) in

  let check_bounds row col =
    row < gridRows && row >= 0 && col < gridCols && col >= 0
  in

  let rec step_in_direction letter_index =
    let newRow = row + (letter_index * fst direction) in
    let newCol = col + (letter_index * snd direction) in
    match (check_bounds newRow newCol, letter_index) with
    | _, n when n == target_len -> true
    | false, _ -> false
    | true, _ ->
        let ch = (List.nth grid newRow).[newCol] in
        if ch != str.[letter_index] then false
        else step_in_direction (letter_index + 1)
  in
  step_in_direction 0

let find_string_in_directions row col str grid =
  let directions =
    [ (-1, -1); (-1, 0); (-1, 1); (0, -1); (0, 1); (1, -1); (1, 0); (1, 1) ]
  in

  let rec check_directions directions acc =
    match directions with
    | [] -> acc
    | h :: t ->
        if find_string_in_direction row col str grid h then
          check_directions t (acc + 1)
        else check_directions t acc
  in

  check_directions directions 0

let find_mas_around_letter row col grid =
  let ch = (List.nth grid row).[col] in
  if ch != 'A' then false
  else
    let directions = [ (-1, -1); (-1, 1); (1, -1); (1, 1) ] in
    let opposite = [ (1, 1); (1, -1); (-1, 1); (-1, -1) ] in

    let is_mas_around direction opposite =
      let one = (List.nth grid (row + fst direction)).[col + snd direction] in
      let two = (List.nth grid (row + fst opposite)).[col + snd opposite] in
      (one == 'M' && two == 'S') || (one == 'S' && two == 'M')
    in

    let rec iterateDirections d o =
      match (d, o) with
      | h1 :: t1, h2 :: t2 ->
          if is_mas_around h1 h2 then iterateDirections t1 t2 else false
      | [], [] -> true
      | _ -> false
    in

    iterateDirections directions opposite

let silver () =
  let input = read_input "../inputs/04.txt" in
  let gridRows = List.length input in
  let gridCols = String.length (List.nth input 0) in

  let rec iterateGrid row col acc =
    match (row, col) with
    | row, _ when row == gridRows -> acc
    | _, col when col == gridCols -> iterateGrid (row + 1) 0 acc
    | row, col ->
        iterateGrid row (col + 1)
          (acc + find_string_in_directions row col "XMAS" input)
  in

  iterateGrid 0 0 0

let gold () =
  let input = read_input "../inputs/04.txt" in
  let gridRows = List.length input in
  let gridCols = String.length (List.nth input 0) in

  let rec iterateGrid row col acc =
    match (row, col) with
    | row, _ when row == gridRows - 1 -> acc
    | _, col when col == gridCols - 1 -> iterateGrid (row + 1) 1 acc
    | row, col ->
        let is_mas_around = find_mas_around_letter row col input in
        iterateGrid row (col + 1) (acc + if is_mas_around then 1 else 0)
  in

  iterateGrid 1 1 0
;;

print_int (gold ())
