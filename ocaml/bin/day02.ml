let read_input filename = 
  let in_channel = open_in filename in
  let lines = In_channel.input_lines in_channel in
  close_in in_channel;

  let parse_line (line: string): int list = 
    let numbers = String.split_on_char ' ' line in
    List.map (fun s -> int_of_string s) numbers
  in

  List.map (fun line -> parse_line line) lines


let is_report_safe (report: int list): bool = 

  let rec valid_pair pair increasing =
    match ((fst pair - snd pair), increasing) with 
    | (diff, _) when abs diff > 3 || abs diff == 0 -> false
    | (diff, incr) -> if incr then diff < 0 else diff > 0
  in

  let rec iterate l increasing =
    match (l, increasing) with 
    | ([], _) -> true
    | (h1::h2::t, None) -> 
      let incr = h1 < h2 in
      let pair_valid = if incr then valid_pair (h1, h2) incr else valid_pair (h1, h2) false in
      if pair_valid then iterate (h2::t) (Some incr) else false
    | (h1::h2::t, Some incr) -> 
      let pair_valid = if incr then valid_pair (h1, h2) incr else valid_pair (h1, h2) false in
      if pair_valid then iterate (h2::t) (Some incr) else false
  in

  iterate report None


let silver = 
  let input = read_input "../inputs/02.txt" in

  let res = List.fold_left (fun sum v -> sum + (if is_report_safe v then 1 else 0)) 0 input in

  print_int res


let () =
  silver
