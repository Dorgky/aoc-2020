(ns aoc.core
  (:require [clojure.string :as str]))

(defn- get-next-location [password search-char start-index pos-list]
  (let [search-fn (partial get-next-location password search-char)
        found-pos (str/index-of password search-char start-index)]
    (if found-pos
      (->> (list found-pos)
           (concat pos-list)
           (search-fn (inc found-pos)))
      pos-list)))

(defn char-locations [password search-char]
  (get-next-location password search-char 0 (list)))

(defn count-characters [password search-char]
  (->> (map str (.toCharArray password))
       (filter #(= search-char %))
       (count))) 

(defn valid-password? [policy password]
  (let [policy-data (str/split policy #"-")
        min-repetitions (Integer/parseInt (first policy-data))
        max-repetitions (Integer/parseInt (second policy-data))
        search-char (first (str/split (last policy-data) #":"))
        char-count (count-characters password search-char)
        position-list (char-locations password search-char)]
    ; Part 1
    (and 
        (<= min-repetitions char-count)
        (>= max-repetitions char-count))
    ; Part 2
    (->> (filter #(or (= min-repetitions (inc %))
                      (= max-repetitions (inc %))) position-list)
         count
         (= 1))))

(defn count-valid-passwords [matched-so-far next-password]
  (let [password-data (str/split next-password #" ")
        policy (str/join "-" (list (first password-data) 
                                   (second password-data)))
        password (last password-data)]
    (if (valid-password? policy password)
      (inc matched-so-far)
      matched-so-far)))

; Real inputs
#_(let [inputs (slurp "./inputs.txt")]
    (reduce count-valid-passwords 0 (str/split-lines inputs)))

; Sample inputs
#_(let [inputs (str "1-3 a: abcde\n"
                    "1-3 b: cdefg\n"
                    "2-9 c: ccccccccc")]
    (reduce count-valid-passwords 0 (str/split-lines inputs)))
