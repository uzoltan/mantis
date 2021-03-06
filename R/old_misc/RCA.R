# A "merged_important"-b�l indulunk ki "materials"-ok alapj�n,
# a meteralsokat aggreg�ltan kezelj�k (figyelt id�tartam: [els�csere-delta, utols� csere])

#f�ggv�nyn�v �s param�terek - Truck Error Signal Finder (merged_important, )
# El�sz�r az �tvett t�bl�t aggreg�ljuk a "materials"-ok �s az "identifier"-ek alapj�n, l�trehozunk �j oszlopot az els� �s utols� cser�nek 
library(stringr)
library(timevis)

#filtering further the "important" parts for RCA purposes
im.parts <- c("Filter", "Hose / Hose parts", "Chain", "Electrical Control Unit", "Electrical component")
important.filtered <- important_replacements[which(str_detect(important_replacements$Type, paste(im.parts, collapse = '|'))),]

all.data <- still[,c("identifier","metatimestamp","metamachinerrorcode")]

plotter <- subset(all.data, !is.na(all.data$metamachinerrorcode) & 
                    (all.data$identifier == important.filtered[4, "identifier"]) & 
                    ((as.Date(all.data$metatimestamp) <= as.Date(important.filtered[4,"metatimestamp"])) &
                       (as.Date(all.data$metatimestamp) >= as.Date(as.Date(important.filtered[4, "metatimestamp"]) - 15))  ))
plotter$style<-"color:blue;"

#add maintenance date - ide j�n a truck, a csereid�pontja �s az alkatr�sz
plotter$metatimestamp <- as.character(plotter$metatimestamp)
plotter[nrow(plotter)+1, ] <- c("515063B00279", "2013-07-03 20:00:00", "0174164","color:orange;")
plotter$metatimestamp <- as.factor(plotter$metatimestamp)

plotter["type"] <- "point"

names(plotter)[names(plotter)=="metatimestamp"] <- "start"
names(plotter)[names(plotter)=="metamachinerrorcode"] <- "content"


timevis(plotter, showZoom = FALSE, fit = TRUE, options = list(editable = TRUE, height = "470px"), elementId = "hey")
