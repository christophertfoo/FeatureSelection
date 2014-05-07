require('rpart')

createFolds <- function(data, nfolds=10) {
  folds <- list()
  indexes <- split(sample(1:nrow(data)), 1:nfolds)
  for(i in 1:length(indexes)) {
    folds[[i]] <- data[indexes[[i]],] 
  }
  return(folds)
}

tenFold <- function(folds, features, class_col='class') {
  count_match <- numeric()
  count_total <- numeric()
  for(i in 1:10) {
    test <- folds[[i]]
    training <- NULL
    for(j in 1:10) {
      if(i != j) {
        if(is.null(training)) {
          training <- folds[[j]]
        } else {
          training <- rbind(training, folds[[j]])
        }
      }
    }
    model <- rpart(as.formula(paste(class_col, " ~ ", paste(features, collapse='+'), sep="")), training)
    results <- predict(model, test, type="class")
    
    count_total <- c(count_total, nrow(test))
    temp_match <- 0
    for(j in 1:nrow(test)) {
      if(results[j] == test[j, class_col]) {
        temp_match <- temp_match + 1
      }
    }
    
    count_match <- c(count_match, temp_match)
  }
  return(data.frame(matches=count_match, total=count_total))
}

averageAccuracy <- function(result) {
  percent <- function(x) {
    x[['matches']] / x[['total']] * 100
  }
  
  return(mean(apply(result, 1, percent)))
}

car <- read.csv('car.csv')
car_folds <- createFolds(car)

writeLines('Car - All')
result <- tenFold(car_folds, setdiff(names(car), c("class")))
writeLines(paste(averageAccuracy(result), "%", sep=""))

writeLines('Car - FCBF')
result <- tenFold(car_folds, c('safety', 'persons'))
writeLines(paste(averageAccuracy(result), "%", sep=""))

writeLines('Car - MVM')
result <- tenFold(car_folds, c('doors', 'lug_boot', 'maint', 'buying'))
writeLines(paste(averageAccuracy(result), "%", sep=""))


mushroom <- read.csv('mushroom.csv')
mushroom_folds <- createFolds(mushroom)
writeLines('Mushroom - All')
result <- tenFold(mushroom_folds, setdiff(names(mushroom), c("class")))
writeLines(paste(averageAccuracy(result), "%", sep=""))

writeLines('Mushroom - FCBF')
result <- tenFold(mushroom_folds, c('odor', 'Spore.print.color', 'Stalk.surface.above.ring', 'Gill.size'))
writeLines(paste(averageAccuracy(result), "%", sep=""))

writeLines('Mushroom - MVM')
result <- tenFold(mushroom_folds, c('Veil.type', 'Stalk.shape', 'Gill.attachment', 'Veil.color'))
writeLines(paste(averageAccuracy(result), "%", sep=""))