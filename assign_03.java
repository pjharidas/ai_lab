    import java.util.Scanner;

    class VariableAssignment {
        char variable;
        int value;
    }

    public class assign_03 {

        static int[] usedDigits = new int[10];
        static int count   = 0;
        static boolean isSolved(VariableAssignment[] assignments, String[] words) {
            int[] values = new int[words.length];

            for (int i = 0; i < words.length; i++) {
                String word = words[i];
                int value = 0;

                for (int j = 0; j < word.length(); j++) {
                    char character = word.charAt(j);
                    for (VariableAssignment assignment : assignments) {
                        if (assignment.variable == character) {
                            value = value * 10 + assignment.value;
                            break;
                        }
                    }
                }

                values[i] = value;
            }

            int sum = 0;
            for (int i = 0; i < values.length - 1; i++) {
                sum += values[i];
            }

            return sum == values[values.length - 1];
        }

        static boolean findSolution(VariableAssignment[] assignments, int index, String[] words) {
            if (index == assignments.length) {
                if (isSolved(assignments, words)) {
                    System.out.println("\nSolution No:"+count);
                    for (VariableAssignment assignment : assignments) {
                        System.out.println(assignment.variable + " = " + assignment.value);
                    }
                    count++;
                    return true;
                }
                return false;
            }

            boolean found = false;
            for (int digit = 0; digit < 10; digit++) {
                if (usedDigits[digit] == 0) {
                    assignments[index].value = digit;
                    usedDigits[digit] = 1;
                    found |= findSolution(assignments, index + 1, words);
                    usedDigits[digit] = 0;
                }
            }

            return found;
        }

        static boolean solve(String[] words) {
            String allWords = String.join("", words);
            String uniqueChars = "";
            for (int i = 0; i < allWords.length(); i++) {
                char character = allWords.charAt(i);
                if (uniqueChars.indexOf(character) == -1) {
                    uniqueChars += character;
                }
            }

            if (uniqueChars.length() > 10) {
                System.out.println("Invalid input - Total number of distinct characters should be less than 10\nyou have entered "+uniqueChars.length()+" distinct characters");
                return false;
            }

            VariableAssignment[] assignments = new VariableAssignment[uniqueChars.length()];

            for (int i = 0; i < assignments.length; i++) {
                assignments[i] = new VariableAssignment();
                assignments[i].variable = uniqueChars.charAt(i);
            }

            return findSolution(assignments, 0, words);
        }

        public static void main(String[] args) {
            Scanner scanner = new Scanner(System.in);

            System.out.print("Enter the number of words: ");
            int numWords = scanner.nextInt();
            String[] words = new String[numWords];

            for (int i = 0; i < numWords; i++) {
                System.out.print("Enter word " + (i + 1) + ": ");
                words[i] = scanner.next().toUpperCase();
            }

            if (!solve(words)) {
                System.out.println("No solution found.");
            }else{
                System.out.println("\n\nTotal "+count+" solution Found");
            }
            scanner.close();
        }
    }
