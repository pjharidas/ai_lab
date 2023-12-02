% Define the knowledge base
song('Shape of You', pop, upbeat, happy, 120, male_vocals).
song('Someone Like You', pop, slow, sad, 67, female_vocals).
song('Bohemian Rhapsody', rock, moderate, epic, 72, male_vocals).
song('Stairway to Heaven', rock, slow, epic, 63, male_vocals).
song('Billie Jean', R&B, moderate, upbeat, 117, male_vocals).
song('I Will Always Love You', ballad, slow, romantic, 66, female_vocals).
song('Smells Like Teen Spirit', grunge, fast, angry, 116, male_vocals).
song('This Love', pop, moderate, happy, 100, male_vocals).
song('Shallow', pop, moderate, romantic, 96, mixed_vocals).
song('Circles', pop, moderate, happy, 120, male_vocals).
song('Believer', rock, fast, epic, 125, male_vocals).
song('In The End', rock, fast, epic, 105, male_vocals).
song('All of Me', ballad, slow, romantic, 120, male_vocals).
song('Uptown Funk', funk, fast, happy, 115, male_vocals).
song('Hello', pop, slow, sad, 78, female_vocals).
song('Sugar', pop, fast, happy, 120, male_vocals).
song('Thunderstruck', rock, fast, epic, 74, male_vocals).
song('Dream On', rock, slow, epic, 94, male_vocals).
song('I Want to Dance with Somebody', pop, fast, happy, 118, female_vocals).
song('Purple Rain', rock, slow, romantic, 67, male_vocals).
song('Sweet Child O Mine', rock, moderate, epic, 125, male_vocals).
song('Hotline Bling', R&B, slow, romantic, 97, male_vocals).
song('Faded', electronic, moderate, sad, 88, male_vocals).
song('All About That Bass', pop, moderate, upbeat, 134, female_vocals).

% Define the rules for song recommendation
recommended_song(Genre, Tempo, Mood, Song) :-
  song(Song, Genre, Tempo, Mood, _, _).

recommended_song(Genre, Tempo, Mood, Song) :-
  song(Song, Genre, _, Mood, _, _),
  tempo_match(Tempo, Song).

recommended_song(Genre, Tempo, Mood, Song) :-
  song(Song, Genre, _, _, Duration, _),
  duration_match(Duration, Tempo).

recommended_song(Genre, Tempo, Mood, Song) :-
  song(Song, Genre, _, _, _, Vocals),
  vocals_match(Vocals, Mood).

tempo_match(slow, Song) :-
  song(Song, _, Tempo, _, _, _),
  member(Tempo, [slow, moderate]).

tempo_match(moderate, Song) :-
  song(Song, _, Tempo, _, _, _),
  member(Tempo, [moderate, fast]).

tempo_match(fast, Song) :-
  song(Song, _, Tempo, _, _, _),
  member(Tempo, [fast, upbeat]).

duration_match(Duration, Tempo) :-
  Tempo == slow,
  Duration > 60,
  Duration < 80.

duration_match(Duration, Tempo) :-
  Tempo == moderate,
  Duration > 80,
  Duration < 110.

duration_match(Duration, Tempo) :-
  Tempo == fast,
  Duration > 110,
  Duration < 140.

vocals_match(male_vocals, Mood) :-
  Mood == upbeat.

vocals_match(female_vocals, Mood) :-
  Mood == sad.

vocals_match(mixed_vocals, Mood) :-
  Mood == romantic.

% Define the user interface
recommend_songs :-
write('What is your preferred genre? '),
read(Genre),
write('What is your preferred tempo (slow, moderate, or fast)? '),
read(Tempo),
write('What is your current mood? '),
read(Mood),
recommended_song(Genre, Tempo, Mood, Song),
write('We recommend: '),
write(Song).
