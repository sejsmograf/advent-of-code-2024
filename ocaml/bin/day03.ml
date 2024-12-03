let read_input filename =
  let in_channel = open_in filename in
  let lines = In_channel.input_lines in_channel in
  close_in in_channel;

  List.fold_left (fun acc line -> acc ^ line) "" lines

let get_regex_matches input regex =
  let rec find_matches index acc =
    try
      let match_index = Str.search_forward regex input index in
      find_matches (match_index + 1) (acc @ [ Str.matched_string input ])
    with Not_found -> acc
  in
  find_matches 0 []

let silver =
  let input = read_input "../inputs/03.txt" in
  let regex = Str.regexp "mul(\\([0-9]*\\),\\([0-9]*\\))" in
  let matches = get_regex_matches input regex in

  List.fold_left
    (fun a (x, y) -> a + x * y)
    0
    (List.map
       (fun m -> Scanf.sscanf m "mul(%d,%d)" (fun a b -> (a, b)))
       matches)
;;

print_int silver
