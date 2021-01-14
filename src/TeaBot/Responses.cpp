
#include "Responses.hpp"

#include <cstdio>
#include <errno.h>
#include <cstring>

namespace TeaBot {

/**
 * @param td_api::updateNewMessage update
 * @param TdLibHandler             *handler
 *
 * Constructor.
 */
Responses::Responses(td_api::updateNewMessage update, TdLibHandler *handler):
        update_(std::move(update))
    ,   handler_(handler)
{
}


/**
 * @return void
 */
void Responses::run()
{
    chat_id_ = update_.message_->chat_id_;
    td_api::downcast_call(
        *(update_.message_->sender_),
        overloaded(
            [this](td_api::messageSenderUser &user) {
                sender_id_   = user.user_id_;
                sender_name_ = handler_->get_user_name(sender_id_);
            },
            [this](td_api::messageSenderChat &chat) {
                sender_id_   = chat.chat_id_;
                sender_name_ = handler_->get_chat_title(sender_id_);
            }
        )
    );

    if (handler_->get_user_id() == sender_id_) {
        handle_self_message();
        return;
    }
}

#include "Responses/handle_self_message.hpp"

} /* namespace TeaBot */