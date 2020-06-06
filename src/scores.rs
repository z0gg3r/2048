fn main() 
{
    let mut score: usize = 0;
    let mut idx = 0;
    for argu in std::env::args() {
        if idx == 1 {
            score = if argu.parse().is_ok() {
                        argu.parse().unwrap()
                } else {
                        0
                };
        }
        idx += 1;
    }
    let mut scores = read_scores();
    scores.push(score);
    scores = sort_scores(scores);
    write_scores(scores);
}

fn read_scores()
    -> Vec<usize>
{
    let file = std::path::Path::new("SCORES");
    if !file.exists() {
        std::fs::write("SCORES", "").unwrap();
    }
    let score_file = std::fs::read_to_string("SCORES");
    if score_file.is_err() {
        eprintln!("{}", score_file.err().unwrap());
        let v: Vec<usize> = vec![0, 0, 0];
        return v;
    }
    let score_file = score_file.unwrap();
    let score_string = score_file.split("\n");
    let mut v: Vec<usize> = Vec::new();
    for s in score_string {
        v.push(s.parse().unwrap());
    }
    v
}

fn write_scores(scores: Vec<usize>)
{
    let mut s = String::from("");
    for i in scores {
        s += &i.to_string();
        s += "\n";
    }
    if std::fs::write("SCORES", &s).is_err() {
        eprintln!("Couldn't write to scores file.");
    }
}

fn sort_scores(mut scores: Vec<usize>)
    -> Vec<usize>
{
    scores.sort();
    if scores.len() > 20 {
        scores.resize(20, 0);
    }
    scores
}
