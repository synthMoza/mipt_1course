#pragma once

//! Counts the size of the ile in bytes
//! @param ptr The pointer to the file
//! @return The size of the file
#define FILE_SIZE(ptr)                                                                          \
({                                                                                              \
    fseek((ptr), 0, SEEK_END);                                                                  \
    unsigned long size = ftell((ptr));                                                          \
    rewind((ptr));                                                                              \
    size;                                                                                       \
})

#define WELCOME_AKINATOR                                                                        \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t\tТакое маленькое ничтожество как ты посмело усомниться\n");                      \
    printf("\t\t\t\t\tв моей гениальноcти, Akinator?\n");                                       \
};

#define AKINATOR_ASK                                                                            \
{                                                                                               \
    printf("/=================================Akinator=================================/\n");   \
};

#define AKINATOR_GUESS(str)                                                                     \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t\tХа! Я знаю, о чем ты говоришь! Это %s!\n", (str));                              \
    printf("\t\t\tЯ же прав, ведь так ?!\n");                                                   \
};

#define AKINATOR_PLAY                                                                           \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t\tЧто ж, это смелое решение. Но усилия напрасны - я слишком силен!\n");           \
};

#define AKINATOR_DECIDE_NOT_TO_PLAY                                                             \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t\tХа-ха-ха! Я так и знал, что ты откажешься! Я уже давно победил!\n");            \
    printf("\t\t\t\tНо ты, мелюзга, не имеешь право мне отказать!\n");                          \
    printf("\t\tЯ буду наслаждаться очередной победой над маленькими человеками!\n");           \
};

#define AKINATOR_TROUBLED                                                                       \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t\tЧто это значит? Твой грязный и никчемный язык я не понимаю!\n");                \
    printf("\t\t\tОтвечай прямо, коротко и ясно - \"да\" или \"нет\"!\n");                      \
    printf("\t\t  Впрочем, это неважно! Ты все раво сыграешь со мной!\n");                      \
};

#define AKINATOR_START_GAME                                                                     \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t\tДа начнется битва! Хотя и битвой это не назвать. Так, избиение\n");             \
};

#define AKINATOR_CORRECT_GUESS                                                                  \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t\tХа! Я так и знал! Я никогда не проигрываю!\n");                                 \
    printf("\t\t\t\tТы проиграл!\n");                                                           \
};

#define AKINATOR_WRONG_GUESS                                                                    \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t\tНет! НЕ МОЖЕТ БЫТЬ! ТЫ ЛЖЕШЬ! Я НЕ МОГ ПРОИГРАТЬ!\n");                          \
};

#define AKINATOR_THINKING                                                                       \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t\tХмм... Дай-ка мне подумать секундочку...\n");                                   \
};
#define AKINATOR_ADDING_NEW_VALUE                                                               \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t\tНесмотря на твое жульничиство, я все же поинтересуюсь, что \n");                \
    printf("\t\t\t\tже был за правильный ответ.\n");                                            \
};
#define AKINATOR_NEW_VALUE_QUESTION                                                             \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\tИ какой же отличительный признак вашего персонажа?\n");                           \
};
#define WAITING_FOR_THE_ANSWER                                                                  \
{                                                                                               \
    printf("Твой ответ: \n");                                                                   \
};
#define AKINATOR_BECAME_STRONGER                                                                \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t\tХа-ха! Теперь я стал сильнее! Готовься к следующей битве!\n");                  \
    printf("\t\t\tТЫ ПРОИГРАЕШЬ!\n");                                                           \
};

#define AKINATOR_START_DIALOGUE                                                                 \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t Приветствую, странник. Не желаешь ли узнать что-то новое про тех, \n");          \
    printf("\tкого ты думал, что знаешь? Или хочешь, чтобы я угадал его САМ?\n");               \
    printf("\t\t(1 - узнать новое про персонажа, 2 - играть в Акинатора)\n");                   \
};

#define AKINATOR_ASKS_A_CHARACTER                                                               \
{                                                                                               \
    AKINATOR_ASK                                                                                \
    printf("\t\tИ о ком же заблудшая душа хочет узнать все?\n");                                \
};

#define AKINATOR_DEATH                                                                          \
{                                                                                               \
    AKINATOR_ASK;                                                                               \
    printf("\t\tЯ ВЕЛЕЛ ТЕБЕ РАЗГОВАРИВАТЬ НОРМАЛЬНО!\n");                                      \
    printf("\t\tТы бесполезен, я ухожу.");                                                      \
};