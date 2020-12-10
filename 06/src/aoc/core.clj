(ns aoc.core
  (:require [clojure.string :as str]))

(defn find-global-answers [data next-line]
  (let [letters (into [] (map str (.toCharArray next-line)))]
    (if (:new-group data)
      (-> data 
          (assoc-in [:letters] letters)
          (assoc-in [:new-group] false))
      (reduce 
        (fn [new-data next-letter]
          (if (not (some #(= next-letter %) letters))
            (assoc-in new-data [:letters] (remove #(= next-letter %) (:letters new-data)))
            new-data)) data (:letters data)))))

(defn find-distinct-answers [data next-line]
  (let [letters (map str (.toCharArray next-line))]
    (reduce 
      (fn [new-data next-letter]
        (if (not (some #(= next-letter %) (:letters new-data)))
          (-> new-data
              (update-in [:current] inc)
              (update-in [:letters] concat [next-letter]))
          new-data)) data letters)))

(defn process-lines [answer next-line]
  (if (str/blank? next-line)
    (-> answer 
        (update-in [:total] + (:current answer))
        (update-in [:global-total] + (count (:letters answer)))
        (assoc-in [:new-group] true)
        (assoc-in [:current] 0)
        (assoc-in [:letters] []))
    (->> (find-global-answers answer next-line)
         (merge answer))))

#_(let [inputs (-> (slurp "./inputs.txt")
                   (str/split-lines))]
    (let [answer 
          (reduce 
            process-lines 
            {:new-group true
             :total 0
             :global-total 0
             :current 0
             :letters []}
            inputs)]
      (-> answer
          (update-in [:total] + (:current answer))
          (update-in [:global-total] + (count (:letters answer))))))
