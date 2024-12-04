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

let extract_integers str = Scanf.sscanf str "mul(%d,%d)" (fun a b -> (a, b))

let extract_integers_and_mul str =
  let ints = extract_integers str in
  fst ints * snd ints

let silver () =
  let input = read_input "../inputs/03.txt" in
  let regex = Str.regexp "mul(\\([0-9]*\\),\\([0-9]*\\))" in
  let matches = get_regex_matches input regex in

  List.fold_left (fun a str -> a + extract_integers_and_mul str) 0 matches

let gold () =
  let input = read_input "../inputs/03.txt" in
  let regex = Str.regexp "mul(\\([0-9]*\\),\\([0-9]*\\))\\|do()\\|don't()" in
  let matches = get_regex_matches input regex in

  let rec summarize l enabled acc =
    match (l, enabled) with
    | [], _ -> acc
    | h :: t, _ when String.starts_with ~prefix:"don" h -> summarize t false acc
    | h :: t, _ when String.starts_with ~prefix:"do(" h -> summarize t true acc
    | h :: t, true -> summarize t true (acc + extract_integers_and_mul h)
    | _ :: t, false -> summarize t false acc
  in

  summarize matches true 0
;;

print_int (gold ())
